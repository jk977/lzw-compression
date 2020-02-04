#include "outstream.h"

#include <stdlib.h>

struct outstream {
    void (*write)(unsigned char, void*);
    void* context;

    unsigned int outcount;

    long int buffer;
    unsigned int bufsize;
};

struct outstream* outs_init(void (*write_byte)(unsigned char c, void* context)) {
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

void outs_destroy(struct outstream* outs) {
    free(outs);
}

void outs_write_bits(struct outstream* outs, long int bits, size_t bit_count) {
}

void outs_flush(struct outstream* outs) {
}
