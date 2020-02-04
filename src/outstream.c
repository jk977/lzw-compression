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
struct outstream* outs_init(void (*write_byte)(unsigned char c, void* context))
{
    struct outstream* outs = malloc(sizeof(*outs));

    if (outs == NULL) {
        return NULL;
    }

    outs->write = write_byte;
    outs->context = context;

    outs->bytes_written = 0;
    outs->buffer = 0;
    outs->bufsize = 0;

    return outs;
}

/*
 * outs_destroy: Free the structure created by outs_init().
 */
void outs_destroy(struct outstream* outs)
{
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
    size_t pending_bits = bit_count + outs->bufsize;

    while (pending_bits >= CHAR_BIT) {
        // size of the empty bits in buffer
        size_t const lower_size = CHAR_BIT - outs->bufsize;

        // ensure that outs->buffer and bits don't contain garbage by
        // zeroing unimportant bits
        unsigned char const upper_mask = (~0 << lower_size);
        unsigned char const lower_mask = ~upper_mask;

        // put the old buffer bits into the upper part of the output,
        // and the parameter bits into the lower part, as specified by
        // the assignment description
        outs->buffer = (outs->buffer & upper_mask) | (bits & lower_mask);
        outs->bufsize = CHAR_BIT;

        outs_flush(outs);

        // update information about the remaining bits.
        // note: bitshifting signed integers is technically undefined behavior.
        pending_bits -= CHAR_BIT;
        bits >>= lower_size;
    }

    if (pending_bits >= 0) {
        // store remaining bits in buffer, adding trailing zeroes
        outs->buffer = (bits << CHAR_BIT - pending_bits);
    }

    outs->bufsize = pending_bits;
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

    outs->buffer = '\0';
    outs->bufsize = 0;
    ++outs->outcount;
}
