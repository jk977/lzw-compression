#include "lzw.h"

#include "bitops.h"
#include "lzwcontext.h"
#include "types.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <limits.h>
#include <string.h>

/*
 * verify_params: Ensure that the given parameters are valid.
 */
static bool verify_params(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void* context),
        void (*write_byte)(unsigned char c, void* context))
{
    return start_bits >= LZW_MINIMUM_BITS
        && max_bits <= LZW_MAXIMUM_BITS
        && read_byte != NULL
        && write_byte != NULL;
}

/*
 * write_prev_char_code: Looks up the string (i.e., sequence) in the trie and
 *                       writes its code if found. Returns false if not found.
 */
static bool write_prev_char_code(struct lzwcontext* ctx, unsigned int current_bits, char const* str, size_t length)
{
    code_t* code = trie_lookup(ctx->trie, str, length);

    if (code == NULL) {
        return false;
    }

    outs_write_bits(ctx->outs, *code, current_bits);
    return true;
}

/*
 * lzwEncode: Encode the bytes read via read_byte using LZW compression
 *            with variable-width codes, writing the result via write_byte.
 */
bool lzwEncode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void*),
        void (*write_byte)(unsigned char, void*),
        void* context)
{
    if (!verify_params(start_bits, max_bits, read_byte, write_byte)) {
        return false;
    }

    size_t const init_seq_size = 1;
    struct lzwcontext* ctx = context_init(init_seq_size, context, read_byte, write_byte);

    if (ctx == NULL) {
        return false;
    }

    int next;
    code_t next_code = TRIE_CHILDREN_COUNT;

    char* current_str = NULL;
    unsigned int current_bits = start_bits;

    while ((next = ins_read_bits(ctx->ins, CHAR_BIT)) != EOF) {
        seq_push(ctx->seq, next);
        current_str = seq_to_cstr(ctx->seq);

        // if sequence isn't found, write its prefix's code and
        // assign the sequence a new character code if possible
        if (!trie_contains(ctx->trie, current_str, seq_length(ctx->seq))) {
            if (!write_prev_char_code(ctx, current_bits, current_str, seq_length(ctx->seq) - 1)) {
                return false;
            }

            int32_t const code_max = ~0u >> (BITS_IN(code_max) - current_bits);
            bool needs_expand = next_code == code_max;
            bool can_expand = current_bits < max_bits;

            // expand the current code width by 1 if it's possible and needed
            if (needs_expand && can_expand) {
                trie_insert(ctx->trie, current_str, seq_length(ctx->seq), next_code);
                ++current_bits;
                ++next_code;
            }

            // restart sequence at current character
            seq_clear(ctx->seq);
            seq_push(ctx->seq, next);
        }

        free(current_str);
    }

    // do one last code write before returning
    char* final_str = seq_to_cstr(ctx->seq);

    if (!write_prev_char_code(ctx, current_bits, final_str, seq_length(ctx->seq))) {
        return false;
    }

    free(final_str);
    context_destroy(ctx);

    return true;
}

/*
 * lzwDecode: Decode the bytes read via read_byte using LZW compression
 *            with variable-width codes, writing the result via write_byte.
 */
bool lzwDecode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void*),
        void (*write_byte)(unsigned char, void*),
        void* context)
{
    if (!verify_params(start_bits, max_bits, read_byte, write_byte)) {
        return false;
    }

    struct lzwcontext* ctx = context_init(1, context, read_byte, write_byte);

    if (ctx == NULL) {
        return false;
    }

    code_t current_code;
    unsigned int current_bits = start_bits;

    while ((current_code = ins_read_bits(ctx->ins, current_bits)) != EOF) {
        // TODO: decode sequence of codes and write them 1 byte at a time
        unsigned char next_output = '\0';
        outs_write_bits(ctx->outs, next_output, CHAR_BIT);
    }

    context_destroy(ctx);
    return false;
}
