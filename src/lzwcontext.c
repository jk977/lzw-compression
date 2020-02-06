#include "lzwcontext.h"

#include <stdlib.h>
#include <string.h>

/*
 * create_trie: Initialize the trie to be used in the LZW compression algorithm.
 */
static struct trie* create_trie(void)
{
    struct trie* trie = trie_init(-1);

    for (size_t i = 0; i < CHILDREN_COUNT; ++i) {
        char key = (char) i;

        if (!trie_insert(trie, &key, 1, i)) {
            trie_destroy(trie);
            return NULL;
        }
    }

    return trie;
}

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
    ctx->trie = create_trie();

    ctx->seq = malloc(seq_length);
    ctx->seq_length = seq_length;

    if (ctx->outs == NULL
            || ctx->ins == NULL
            || ctx->trie == NULL
            || ctx->seq == NULL) {
        context_destroy(ctx);
        return NULL;
    }

    memset(ctx->seq, 0, seq_length);
    return ctx;
}

/*
 * context_destroy: Free the structure initialized by context_init().
 */
void context_destroy(struct lzwcontext* ctx)
{
    ins_destroy(ctx->ins);
    outs_destroy(ctx->outs);
    trie_destroy(ctx->trie);

    free(ctx->seq);
    free(ctx);
}

