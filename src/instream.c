#include "instream.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <limits.h>

#include <sys/types.h>

#define BITS_IN(x) (CHAR_BIT * sizeof(x))

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
 * add_to_buffer: Add the given byte to the buffer in the input byte stream.
 *                If the buffer cannot contain the entire byte, the return
 *                value is the old buffer filled with as many bytes as possible.
 *                Otherwise, the return value is EOF.
 */
static int32_t add_to_buffer(struct instream* ins, unsigned char byte, size_t bit_count)
{
    int32_t result = EOF;

    if (bit_count > ins->bufsize + CHAR_BIT) {
        // add to buffer without emptying
        size_t const shift_distance = BITS_IN(ins->buffer) - ins->bufsize - CHAR_BIT;
        ins->buffer |= byte << shift_distance;
        ins->bufsize += CHAR_BIT;
    } else {
        // enough bits have been read for the buffer to fill

        // extract the used and unused (overflow) bits
        // in a right-aligned 0-padded 32-bit space
        unsigned char const ones = (unsigned char) ~0u;
        unsigned char const overflow_mask = ones >> (bit_count - ins->bufsize);

        uint32_t used_bits = (int32_t) (byte & ~overflow_mask);
        uint32_t unused_bits = (int32_t) (byte & overflow_mask);

        // how much to pad the used and unused bits on the right when shifting them
        size_t const used_bit_padding = BITS_IN(ins->buffer) - ins->bufsize - CHAR_BIT;
        size_t const unused_bit_padding = BITS_IN(ins->buffer) + bit_count - ins->bufsize - CHAR_BIT;

        // align with the end of buffer data
        used_bits <<= used_bit_padding;

        // align to the very left and avoid runtime errors
        // caused by shifting n-bit integers by n bits
        if (unused_bit_padding >= BITS_IN(unused_bits)) {
            unused_bits = 0;
        } else {
            unused_bits <<= unused_bit_padding;
        }

        // combine buffer and used bits
        result = ins->buffer | used_bits;
        ins->buffer = unused_bits;
        ins->bufsize += CHAR_BIT - bit_count;
    }

    return result;
}

/*
 * ins_read_bits: Read the given number of bits.
 */
int32_t ins_read_bits(struct instream* ins, size_t bit_count)
{
    if (bit_count == 0) {
        // do nothing when asked to read 0 bits
        return EOF;
    } else if (bit_count > BITS_IN(ins->buffer)) {
        // impossible to return more bits than space allows
        return EOF;
    }

    // start result at EOF if nothing in buffer
    int32_t result = EOF;
    size_t bits_remaining = bit_count - ins->bufsize;

    while (bits_remaining > 0) {
        int next = (ins->read)(ins->context);

        if (next == EOF) {
            // no more data to be read; exit loop early
            break;
        }

        result = add_to_buffer(ins, next, bit_count);

        if (result == EOF) {
            bits_remaining -= CHAR_BIT;
        } else {
            bits_remaining = 0;
        }

    }

    if (result != EOF) {
        // right-align result before returning to make it
        // function properly with variable-width codes
        result >>= (BITS_IN(result) - bit_count);
    } else if (ins->bufsize > 0) {
        // take the rest from the buffer if the end is reached
        result = ins->buffer >> (BITS_IN(result) - ins->bufsize);
        ins->buffer = 0;
        ins->bufsize = 0;
    }

    return result;
}
