#include "instream.h"
#include "bitops.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <limits.h>

#include <sys/types.h>

struct instream {
    int (*read)(void*);
    void* context;

    uint32_t buffer;
    size_t bufsize;

    uint32_t incount;
};

/*
 * ins_init: Initialize an input bitstream on the heap.
 */
struct instream* ins_init(void* context,
        int (*read_bits)(void* context))
{
    struct instream* ins = malloc(sizeof(*ins));

    if (ins == NULL) {
        return NULL;
    }

    ins->read = read_bits;
    ins->context = context;

    ins->buffer = 0;
    ins->bufsize = 0;
    ins->incount = 0;

    return ins;
}

/*
 * ins_destroy: Free the structure allocated by ins_init().
 */
void ins_destroy(struct instream* ins)
{
    free(ins);
}

/*
 * insert_buffer: Puts the byte into the input stream buffer, assuming
 *                there will be no overflow.
 */
static void insert_buffer(struct instream* ins, unsigned char byte)
{
    size_t const shift_distance = BITS_IN(ins->buffer) - ins->bufsize - CHAR_BIT;
    ins->buffer |= byte << shift_distance;
    ins->bufsize += CHAR_BIT;
}

/*
 * flush_buffer: If the buffer holds at least bit_count bits,
 *               collects the data stored in the buffer, clears the buffer,
 *               and returns the data. Returns EOF otherwise.
 */
static int32_t flush_buffer(struct instream* ins, size_t bit_count)
{
    if (ins->bufsize < bit_count) {
        return EOF;
    }

    uint32_t const mask = ~0u << (BITS_IN(ins->buffer) - bit_count);
    uint32_t result = ins->buffer & mask;

    ins->buffer <<= bit_count;
    ins->bufsize -= bit_count;

    return result;
}

/*
 * insert_buffer_with_overflow: Inserts data into the buffer, flushes, and returns the completed data.
 */
static int32_t insert_buffer_with_overflow(struct instream* ins, unsigned char byte, size_t bit_count)
{
    // extract the used and unused bits in a right-aligned 0-padded 32-bit space
    unsigned char const overflow_mask = (unsigned char) ~0u >> (bit_count - ins->bufsize);

    uint32_t used_bits = (uint32_t) (byte & ~overflow_mask);
    size_t const used_bits_count = bit_count - ins->bufsize;
    size_t const used_bit_padding = BITS_IN(ins->buffer) - ins->bufsize - CHAR_BIT;

    uint32_t unused_bits = (uint32_t) (byte & overflow_mask);
    size_t const unused_bits_count = CHAR_BIT - used_bits_count;
    size_t const unused_bit_padding = BITS_IN(ins->buffer) - unused_bits_count;

    // align to the very left and avoid runtime errors
    // caused by shifting n-bit integers by n bits
    if (unused_bit_padding >= BITS_IN(unused_bits)) {
        unused_bits = 0;
    } else {
        unused_bits <<= unused_bit_padding;
    }

    // combine buffer and used bits
    ins->buffer |= used_bits << used_bit_padding;
    ins->bufsize += used_bits_count;

    int32_t result = flush_buffer(ins, bit_count);

    ins->buffer |= unused_bits >> ins->bufsize;
    ins->bufsize += unused_bits_count;

    return result;
}

/*
 * add_to_buffer: Adds the byte to the input stream buffer. If enough bits are
 *                collected, flush the buffer and store the resulting bits in *result.
 *                Returns the number of bits added to the buffer before flushing.
 */
static size_t add_to_buffer(struct instream* ins, int32_t* result, unsigned char byte, size_t bit_count)
{
    size_t bits_added;

    if (bit_count > ins->bufsize + CHAR_BIT) {
        bits_added = CHAR_BIT;
        insert_buffer(ins, byte);
    } else {
        bits_added = bit_count - ins->bufsize;
        *result = insert_buffer_with_overflow(ins, byte, bit_count);
    }

    return bits_added;
}

/*
 * ins_read_bits: Return the given number of bits from the input stream.
 *                If bit_count is 0 or greater than the space allowed by
 *                the internal buffer, returns EOF.
 */
int32_t ins_read_bits(struct instream* ins, size_t bit_count)
{
    if (bit_count == 0 || bit_count > BITS_IN(ins->buffer)) {
        return EOF;
    }

    int32_t result = EOF;

    // avoid overflow caused by storing negatives in an unsigned integer type.
    size_t bits_remaining = (bit_count >= ins->bufsize) ?
        bit_count - ins->bufsize :
        0;

    while (bits_remaining > 0) {
        int next = (ins->read)(ins->context);

        if (next == EOF) {
            // no more data to be read; exit loop early
            break;
        }

        bits_remaining -= add_to_buffer(ins, &result, next, bit_count);
    }

    if (result == EOF) {
        // take the rest from the buffer if the end of input is reached
        result = flush_buffer(ins, bit_count);
    }

    if (result != EOF) {
        // right-align result before returning
        size_t const shift_distance = BITS_IN(result) + bits_remaining - bit_count;
        result = (uint32_t) result >> shift_distance;
    }

    return result;
}
