#include "lzwcontext.h"

#include <stdlib.h>
#include <string.h>

/*
 * ctx_init: Initialize the context struct to be used in
 *           `lzw_encode()` and `lzw_decode()`.
 */

struct lzwcontext* ctx_init(size_t seq_len, void* stream_ctx,
        int (*read_byte)(void*),
        void (*write_byte)(unsigned char, void*))
{
    struct lzwcontext* ctx = malloc(sizeof(*ctx));

    if (ctx == NULL) {
        return NULL;
    }

    ctx->outs = outs_init(stream_ctx, write_byte);
    ctx->ins = ins_init(stream_ctx, read_byte);
    ctx->seq = seq_init(seq_len);

    if (ctx->outs == NULL || ctx->ins == NULL || ctx->seq == NULL) {
        ctx_destroy(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * ctx_destroy: Free the structure initialized by `ctx_init()`.
 */

void ctx_destroy(struct lzwcontext* ctx)
{
    ins_destroy(ctx->ins);
    outs_destroy(ctx->outs);
    seq_destroy(ctx->seq);

    free(ctx);
}

