#ifndef LZW_CONTEXT_H_
#define LZW_CONTEXT_H_

#include "outstream.h"
#include "instream.h"
#include "trie.h"

#include <stddef.h>

/*
 * helper struct to make the encode and decode functions more readable
 * and reduce boilerplate.
 */
struct lzwcontext {
    struct outstream* outs;
    struct instream* ins;
    struct trie* trie;

    char* seq;
    size_t seq_length;
};

struct lzwcontext* context_init(size_t seq_length, void* context,
        int (*read_byte)(void*),
        void (*write_byte)(unsigned char, void*));

void context_destroy(struct lzwcontext* ctx);

#endif // LZW_CONTEXT_H_
