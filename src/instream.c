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

    int32_t result = EOF;

    while (result == EOF) {
        int next = (ins->read)(ins->context);

        if (next == EOF) {
            // no more data to be read; exit loop early
            break;
        }

        // assume ins->read returns 1 byte at a time, or EOF on error
        unsigned char const next_byte = next;

        if (bit_count > ins->bufsize + CHAR_BIT) {
            // continue adding to buffer without emptying
            size_t const shift_distance = BITS_IN(ins->buffer) - ins->bufsize - CHAR_BIT;
            ins->buffer |= next_byte << shift_distance;
            ins->bufsize += CHAR_BIT;
        } else {
            // enough bits have been read for the result
            size_t const used_bit_count = bit_count - ins->bufsize;
            size_t const unused_bit_count = ins->bufsize + CHAR_BIT - bit_count;

            unsigned char const overflow_mask = (unsigned char) ~0u >> used_bit_count;

            // extract the used and unused (overflow) bits in a 0-padded 32-bit space
            int32_t used_bits = (int32_t) (next_byte & ~overflow_mask);
            int32_t unused_bits = (int32_t) (next_byte & overflow_mask);

            // number of bits padding the original byte
            size_t const empty_count = BITS_IN(ins->buffer) - CHAR_BIT;

            // align used_bits to end of buffer data, and unused_bits to the very left
            used_bits <<= (empty_count - ins->bufsize);
            unused_bits <<= empty_count;

            // combine buffer and used bits, then right-align the result
            result = ins->buffer | used_bits;
            result >>= (BITS_IN(result) - bit_count);

            ins->buffer = unused_bits;
            ins->bufsize = unused_bit_count;
        }
    }

    return result;
}
