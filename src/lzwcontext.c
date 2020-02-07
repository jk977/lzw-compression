#include "lzwcontext.h"

#include <stdlib.h>
#include <string.h>

/*
 * context_init: Initialize the context struct to be used in
 *               lzwEncode and lzwDecode.
 */
struct lzwcontext* context_init(size_t seq_length, void* context,
        int (*read_byte)(void*),
        void (*write_byte)(unsigned char, void*))
{
    struct lzwcontext* ctx = malloc(sizeof(*ctx));

    if (ctx == NULL) {
        return NULL;
    }

    ctx->outs = outs_init(context, write_byte);
    ctx->ins = ins_init(context, read_byte);
    ctx->seq = seq_init(seq_length);

    if (ctx->outs == NULL
            || ctx->ins == NULL
            || ctx->seq == NULL) {
        context_destroy(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * context_destroy: Free the structure initialized by context_init().
 */
void context_destroy(struct lzwcontext* ctx)
{
    ins_destroy(ctx->ins);
    outs_destroy(ctx->outs);
    seq_destroy(ctx->seq);

    free(ctx);
}

