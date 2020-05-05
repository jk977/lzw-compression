/*
 * lzwcontext.h: Helper structure for the implementation of the LZW algorithm.
 *               Because it's meant to reduce boilerplate and manage pointers
 *               rather than being an object-style structure, it is not opaque.
 */

#ifndef LZW_CONTEXT_H_
#define LZW_CONTEXT_H_

#include "outstream.h"
#include "instream.h"
#include "sequence.h"

#include <stddef.h>

/*
 * helper struct to make the encode and decode functions more readable
 * and reduce boilerplate.
 */

struct lzwcontext {
    struct outstream* outs;
    struct instream* ins;
    struct sequence* seq;
};

struct lzwcontext* ctx_init(size_t seq_len, void* stream_ctx,
        int (*read_byte)(void*),
        void (*write_byte)(unsigned char, void*));

void ctx_destroy(struct lzwcontext* ctx);

#endif // LZW_CONTEXT_H_
