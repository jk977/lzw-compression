#include "outstream.h"

#include <stdlib.h>
#include <stdint.h>

#include <limits.h>

struct outstream {
    void (*write)(unsigned char, void*);
    void* context;

    uint32_t outcount;

    uint32_t buffer;
    uint8_t bufsize;
};

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

void outs_destroy(struct outstream* outs)
{
    free(outs);
}

void outs_write_bits(struct outstream* outs, int bits, size_t bit_count)
{
    size_t pending_bits = bit_count + outs->bufsize;

    while (pending_bits >= CHAR_BIT) {
        if (pending_bits > CHAR_BIT) {
            // write the next byte and store the remainder
        } else {
            // write a single byte and don't store anything
        }

        pending_bits -= CHAR_BIT;
    }

    if (pending_bits >= 0) {
        // store remaining bits in buffer
    }

    outs->bufsize += bit_count;
    outs->bufsize %= CHAR_BIT;
}

void outs_flush(struct outstream* outs)
{
    if (outs->bufsize == 0) {
        return;
    }

    // zero out garbage bits from buffer
    uint32_t mask = (~0 >> outs->bufsize);
    outs->buffer &= mask;

    (outs->write)(outs->buffer);

    outs->bufsize = 0;
    ++outs->outcount;
}
