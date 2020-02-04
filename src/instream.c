#include "instream.h"

#include <stdlib.h>

struct instream {
    void (*read)(unsigned char, void*);
    void* context;

    uint32_t buffer;
    uint8_t bufsize;

    uint32_t incount;
};


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

void ins_destroy(struct instream* ins)
{
    free(ins);
}

int32_t ins_read_bits(struct instream* ins, size_t bit_count)
{
    return -1;
}

int32_t ins_flush(struct instream* ins)
{
    if (ins->bufsize == 0) {
        return -1;
    }

    return ins->buffer;
}
