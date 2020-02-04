#include "instream.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <limits.h>

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
        return EOF;
    }

    int32_t result = ins->buffer;
    size_t bits_read = ins->bufsize;

    int next = 0;
    int overflow = 0;

    while (bits_read < bit_count && next != EOF) {
        next = (ins->read)(ins->context);

        if (next == EOF) {
            if (bits_read == 0) {
                result = EOF;
            }

            break;
        }

        unsigned char const next_byte = (unsigned char) next;
        size_t const bits_remaining = bit_count - bits_read;
        bool const has_overflow = bits_remaining < BITS_IN(next);

        size_t const move_distance = has_overflow ?
            BITS_IN(next_byte) - bits_remaining :
            bits_remaining - BITS_IN(next_byte);

        if (has_overflow) {
            // less bits needed than read
            int32_t const overflow_mask = ~0u >> bits_remaining;
            int32_t const overflow_bits = next & overflow_mask;

            result |= next_byte >> move_distance;
            overflow |= overflow_bits << bits_remaining;
        } else {
            // more bits needed than an int has
            result |= next_byte << move_distance;
            overflow = 0;
        }

        bits_read += BITS_IN(next_byte);
    }

    ins->buffer = overflow;
    ins->bufsize = bits_read - bit_count;

    return result;
}
