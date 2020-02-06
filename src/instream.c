#include "instream.h"
#include "bitops.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <limits.h>

#include <sys/types.h>

struct instream {
    int (*read)(void*);
    void* context;

    uint32_t buffer;
    size_t bufsize;
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

    return ins;
}

/*
 * ins_destroy: Free the structure allocated by ins_init().
 */
void ins_destroy(struct instream* ins)
{
    free(ins);
}

static void add_to_buffer(struct instream* ins, unsigned char bits, size_t bit_count)
{
    if (bit_count == 0) {
        return;
    }

    size_t const buffer_avail = BITS_IN(ins->buffer) - ins->bufsize;
    size_t const unused_buffer_space = buffer_avail - bit_count;

    assert(bit_count <= CHAR_BIT);
    assert(bit_count <= buffer_avail);

    size_t const unused_bit_count = CHAR_BIT - bit_count;
    uint32_t const used_bits = bits & (~0u << unused_bit_count);

    if (unused_buffer_space == bit_count) {
        ins->buffer |= SH_RIGHT(used_bits, unused_bit_count);
    } else if (unused_buffer_space < bit_count) {
        ins->buffer |= SH_RIGHT(used_bits, CHAR_BIT - unused_buffer_space);
    } else {
        ins->buffer |= SH_LEFT(used_bits, unused_buffer_space - unused_bit_count);
    }

    ins->bufsize += bit_count;
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

    unsigned int const bits_needed = (bit_count > ins->bufsize) ?
        ins->bufsize :
        bit_count;

    size_t const align_distance = BITS_IN(ins->buffer) - bits_needed;

    uint32_t const mask = SH_LEFT(~0u, BITS_IN(ins->buffer) - bits_needed);
    uint32_t result = SH_RIGHT(ins->buffer & mask, align_distance);

    ins->buffer = SH_LEFT(ins->buffer, bits_needed);
    ins->bufsize -= bits_needed;

    return (int32_t) result;
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
    } else if (ins->bufsize >= bit_count) {
        return flush_buffer(ins, bit_count);
    }

    unsigned int const bits_needed = bit_count - ins->bufsize;

    int32_t result;
    int const next = (ins->read)(ins->context);
    unsigned char const next_byte = next;

    if (next == EOF) {
        return flush_buffer(ins, bit_count);
    }

    if (bits_needed == CHAR_BIT) {
        add_to_buffer(ins, next_byte, CHAR_BIT);
        result = flush_buffer(ins, bit_count);
    } else if (bits_needed < CHAR_BIT) {
        unsigned int const leftover_bit_count = CHAR_BIT - bits_needed;

        add_to_buffer(ins, next_byte, bits_needed);
        result = flush_buffer(ins, bit_count);
        add_to_buffer(ins, SH_LEFT(next_byte, bits_needed), leftover_bit_count);
    } else {
        add_to_buffer(ins, next_byte, CHAR_BIT);
        result = ins_read_bits(ins, bit_count);
    }

    return result;
}
