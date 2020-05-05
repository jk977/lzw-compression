#include "lzw.h"
#include "trie.h"

#include "lzwcontext.h"
#include "config.h"

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
 * create_trie: Initialize the trie to be used in the LZW compression algorithm.
 *              The trie acts as the dictionary in the assignment's given algorithm.
 */

static struct trie* create_trie(void)
{
    struct trie* trie = trie_init(-1);

    // set every child to hold its index's corresponding character value
    for (size_t i = 0; i < LZW_CHAR_RANGE; ++i) {
        char key = (char) i;

        if (!trie_insert(trie, &key, 1, i)) {
            trie_destroy(trie);
            return NULL;
        }
    }

    return trie;
}

/*
 * write_prev_char_code: Looks up the string (i.e., sequence) in the trie and
 *                       writes its code if found. Returns false if not found.
 */

static bool write_prev_char_code(struct lzwcontext* ctx, struct trie* trie,
        unsigned int current_bits, char const* str, size_t length)
{
    code_t* code = trie_lookup(trie, str, length);

    if (code == NULL) {
        return false;
    }

    outs_write_bits(ctx->outs, *code, current_bits);
    return true;
}

/*
 * lzw_encode: Encode the bytes read via read_byte using LZW compression
 *             with variable-width codes, writing the result via write_byte.
 *             context is the context of the file input/output to be passed
 *             to the read/write functions.
 */

bool lzw_encode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void*),
        void (*write_byte)(unsigned char, void*),
        void* context)
{
    if (!verify_params(start_bits, max_bits, read_byte, write_byte)) {
        return false;
    }

    // boilerplate initializations, etc.
    size_t const init_seq_size = 1;
    struct lzwcontext* ctx = context_init(init_seq_size, context, read_byte, write_byte);
    struct trie* trie = create_trie();

    if (ctx == NULL || trie == NULL) {
        context_destroy(ctx);
        trie_destroy(trie);

        return false;
    }

    int next;
    code_t next_code = LZW_CHAR_RANGE;

    char* current_str = NULL;
    unsigned int current_bits = start_bits;

    while ((next = ins_read_bits(ctx->ins, CHAR_BIT)) != EOF) {
        seq_push(ctx->seq, next);
        current_str = seq_to_cstr(ctx->seq);

        // if sequence isn't found, write everything but the last character and
        // assign the full sequence a new character code if possible
        if (!trie_contains(trie, current_str, seq_length(ctx->seq))) {
            if (!write_prev_char_code(ctx, trie, current_bits, current_str, seq_length(ctx->seq) - 1)) {
                return false;
            }

            int32_t const current_code_max = (1 << current_bits) - 1;
            bool code_needs_expand = next_code >= current_code_max;
            bool code_can_expand = current_bits < max_bits;

            if (!code_needs_expand || code_can_expand) {
                if (code_needs_expand) {
                    ++current_bits;
                }

                // add the full sequence to the trie and move to the next code
                trie_insert(trie, current_str, seq_length(ctx->seq), next_code);
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

    if (!write_prev_char_code(ctx, trie, current_bits, final_str, seq_length(ctx->seq))) {
        return false;
    }

    free(final_str);
    trie_destroy(trie);
    context_destroy(ctx);

    return true;
}

/*
 * destroy_table: Free memory allocated in create_table().
 */

static void destroy_table(struct sequence** table, size_t table_size)
{
    for (size_t i = 0; i < table_size; ++i) {
        seq_destroy(table[i]);
    }

    free(table);
}

/*
 * create_table: Create a sequence table for decoding. This table is structured
 *               to hold the indices' corresponding ASCII character in the first
 *               LZW_CHAR_RANGE entries.
 */

static struct sequence** create_table(size_t table_size)
{
    struct sequence** table = malloc(sizeof(*table) * table_size);

    if (table == NULL) {
        return NULL;
    }

    // initialize everything to null to prevent garbage data if allocation fails
    for (size_t i = 0; i < table_size; ++i) {
        table[i] = NULL;
    }

    // initialize the first LZW_CHAR_RANGE entries to their respective characters
    for (size_t i = 0; i < LZW_CHAR_RANGE; ++i) {
        struct sequence* seq = seq_init(1);
        char c = (char) i;

        if (seq == NULL || !seq_push(seq, c)) {
            // clean up table instead of trying to recover on failure
            destroy_table(table, table_size);
            return NULL;
        }

        table[i] = seq;
    }

    return table;
}

/*
 * output_sequence: Output the given sequence to the output bitstream by
 *                  converting it to a string of bytes. Returns true on
 *                  successful write, or else false.
 */

static bool output_sequence(struct lzwcontext* ctx, struct sequence* seq)
{
    char* str = seq_to_cstr(seq);

    if (str == NULL) {
        return false;
    }

    for (size_t i = 0; i < seq_length(seq); ++i) {
        outs_write_bits(ctx->outs, str[i], CHAR_BIT);
    }

    free(str);
    return true;
}

/*
 * lzw_decode: Decode the bytes read via read_byte using LZW compression
 *             with variable-width codes, writing the result via write_byte.
 *             context is the context of the file input/output to be passed
 *             to the read/write functions.
 */

bool lzw_decode(unsigned int start_bits, unsigned int max_bits,
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

    unsigned int current_bits = start_bits;

    // track the max values for the current and max bits for adjusting code width
    int32_t const code_max = (1 << max_bits) - 1;
    int32_t current_code_max = (1 << current_bits) - 1;

    // create a table large enough to hold all potential codes
    ssize_t next_index = LZW_CHAR_RANGE;
    struct sequence** table = create_table(code_max);

    if (table == NULL) {
        return false;
    }

    // read a single code, output it, and check the code width
    code_t current_code;
    code_t prev_code = ins_read_bits(ctx->ins, current_bits);

    if (!output_sequence(ctx, table[prev_code])) {
        return false;
    }

    if (next_index >= current_code_max) {
        ++current_bits;
    }

    while ((current_code = ins_read_bits(ctx->ins, current_bits)) != EOF) {
        bool const table_has_code = current_code < code_max;
        struct sequence* const current_seq = table_has_code ?
            table[current_code] :
            table[prev_code];

        char c = seq_first(current_seq);

        if (next_index < code_max) {
            // table is not full; add new entry
            struct sequence* const new_entry = seq_copy(table[prev_code]);

            if (new_entry == NULL) {
                return false;
            }

            seq_push(new_entry, c);
            table[next_index] = new_entry;
            ++next_index;
        }

        if (!output_sequence(ctx, table[current_code])) {
            return false;
        }

        prev_code = current_code;
        current_code_max = (1 << current_bits) - 1;

        // expand code width if needed
        if (next_index >= current_code_max) {
            ++current_bits;
        }

    }

    context_destroy(ctx);
    destroy_table(table, code_max);

    return true;
}
