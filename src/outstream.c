#include "outstream.h"
#include "bitops.h"

#include <stdlib.h>
#include <stdint.h>

#include <limits.h>

struct outstream {
    void (*write)(unsigned char, void*);
    void* context;

    unsigned char buffer;
    uint8_t bufsize;
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
void outs_write_bits(struct outstream* outs, uint32_t bits, size_t bit_count)
{
    if (bit_count > BITS_IN(bits)) {
        return;
    }

    size_t const buffer_avail = BITS_IN(outs->buffer) - outs->bufsize;

    if (bit_count == buffer_avail) {
        outs->buffer |= bits;
        outs->bufsize += bit_count;
        outs_flush(outs);
    } else if (bit_count < buffer_avail) {
        outs->buffer |= bits << (buffer_avail - bit_count);
        outs->bufsize += bit_count;
    } else {
        outs->buffer |= bits >> (bit_count - buffer_avail);
        outs->bufsize = CHAR_BIT;
        outs_flush(outs);
        outs_write_bits(outs, bits, bit_count - buffer_avail);
    }
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
}
