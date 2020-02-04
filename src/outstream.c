#include "outstream.h"

#include <stdlib.h>
#include <stdint.h>

#include <limits.h>

struct outstream {
    void (*write)(unsigned char, void*);
    void* context;

    unsigned char buffer;
    uint8_t bufsize;

    uint32_t outcount;
};

/*
 * outs_init: Initialize an output bitstream on the heap.
 */
struct outstream* outs_init(void* context,
        void (*write_byte)(unsigned char c, void* context))
{
    struct outstream* outs = malloc(sizeof(*outs));

    if (outs == NULL) {
        return NULL;
    }

    outs->write = write_byte;
    outs->context = context;

    outs->buffer = 0;
    outs->bufsize = 0;
    outs->outcount = 0;

    return outs;
}

/*
 * outs_destroy: Free the structure allocated by outs_init().
 */
void outs_destroy(struct outstream* outs)
{
    outs_flush(outs);
    free(outs);
}

/*
 * outs_write_bits: Write the given bits.
 *                  If any bits are stored in the buffer, they are added to
 *                  the given bits before writing. Any amount of bits under
 *                  CHAR_BIT are kept in the buffer for the next write.
 */
void outs_write_bits(struct outstream* outs, int bits, size_t bit_count)
{
    size_t bits_pending = bit_count + outs->bufsize;

    while (bits_pending >= CHAR_BIT) {
        // size of the empty bits in buffer
        size_t const lower_size = CHAR_BIT - outs->bufsize;

        // put the old buffer bits into the upper part of the output,
        // and the parameter bits into the lower part, as specified by
        // the assignment description
        unsigned char const upper = outs->buffer & (~0u << lower_size);
        unsigned char const lower = bits >> outs->bufsize;

        outs->buffer = upper | lower;
        outs->bufsize = CHAR_BIT;
        outs_flush(outs);

        // update information about the remaining bits.
        // note: bitshifting signed integers is technically undefined behavior.
        bits_pending -= CHAR_BIT;
        bits >>= lower_size;
    }

    // store remaining bits in buffer, adding trailing zeroes.
    // if no bits remain, buffer will be 0.
    unsigned char const mask = ~0u << (CHAR_BIT - bits_pending + outs->bufsize);
    unsigned char const remaining = bits & mask;

    outs->buffer |= remaining >> outs->bufsize;
    outs->bufsize = bits_pending;
}

/*
 * outs_flush: Flush the buffer of the output bitstream.
 *             Since outs_write_bits() ensures the buffer is properly zeroed,
 *             this function does not check for garbage bits in the buffer.
 */
void outs_flush(struct outstream* outs)
{
    if (outs->bufsize == 0) {
        return;
    }

    (outs->write)(outs->buffer, outs->context);

    outs->buffer = 0;
    outs->bufsize = 0;
    ++outs->outcount;
}
