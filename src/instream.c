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
    uint8_t bufsize;

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

    ssize_t bits_remaining = bit_count - ins->bufsize;
    int32_t result = ins->buffer;

    while (bits_remaining > 0) {
        int next = (ins->read)(ins->context);

        if (next == EOF) {
            // no more data to be read; exit loop early
            if (bits_remaining == (ssize_t) bit_count) {
                result = EOF;
            }

            break;
        }

        // assume ins->read returns 1 byte at a time, or EOF on error
        unsigned char const next_byte = next;

        // check if enough bits have been read for the result
        if (bits_remaining <= (ssize_t) BITS_IN(next_byte)) {
            // extract the bits to be stored in the buffer, zeroing the rest
            size_t const new_bits_shift = BITS_IN(next_byte) - bits_remaining;
            unsigned char const new_bits = next_byte << new_bits_shift;

            // extract the overflow bits, zeroing the rest
            size_t const overflow_shift = BITS_IN(ins->buffer) + BITS_IN(next_byte) - bits_remaining;
            unsigned char const overflow_mask = ~0u >> bits_remaining;
            unsigned char const overflow_bits = next_byte & overflow_mask;

            // find out how much the new bits need to be shifted
            // to align with the rest of the data in the buffer
            size_t const new_shift = BITS_IN(ins->buffer)
                - BITS_IN(next_byte)
                - bit_count + bits_remaining;

            // combine the buffer and new bits into the result
            // and store the aligned overflow bits in the buffer
            result = ins->buffer | (new_bits << new_shift);

            // avoid runtime errors caused by
            // shifting an n-bit integer by n
            if (BITS_IN(ins->buffer) >= overflow_shift) {
                ins->buffer = 0;
            } else {
                ins->buffer = overflow_bits << overflow_shift;
            }

            ins->bufsize = BITS_IN(next_byte) - bits_remaining;
        } else {
            size_t const shift_distance = BITS_IN(ins->buffer)
                - ins->bufsize
                - BITS_IN(next_byte);

            ins->buffer |= next_byte << shift_distance;
            ins->bufsize += BITS_IN(next_byte);
        }

        bits_remaining -= BITS_IN(next_byte);
    }

    return result;
}
