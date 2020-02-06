#include "lzw.h"
#include "lzwcontext.h"
#include "types.h"

#include <stdio.h>
#include <string.h>

#include <limits.h>

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
    code_t next_code = 0;
    unsigned int current_bits = start_bits;

    while ((next = ins_read_bits(ctx->ins, current_bits)) != EOF) {
    }

    context_destroy(ctx);
    return false;
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
