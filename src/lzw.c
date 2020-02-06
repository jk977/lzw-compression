#include "lzw.h"

#include "sequence.h"
#include "trie.h"
#include "outstream.h"
#include "instream.h"

#include "types.h"

#include <stdio.h>
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
 * free_structs: Convenience method to avoid boilerplate frees between encoding
 *               and decoding.
 */
static void free_structs(struct instream* ins, struct outstream* outs, struct sequence* seq, struct trie* trie)
{
    ins_destroy(ins);
    outs_destroy(outs);
    seq_destroy(seq);
    trie_destroy(trie);
}

/*
 * lzwEncode: Encode the bytes read via read_byte using LZW compression
 *            with variable-width codes, writing the result via write_byte.
 */
bool lzwEncode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void* context),
        void (*write_byte)(unsigned char c, void* context),
        void* context)
{
    if (!verify_params(start_bits, max_bits, read_byte, write_byte)) {
        return false;
    }

    struct instream* ins = ins_init(context, read_byte);
    struct outstream* outs = outs_init(context, write_byte);

    struct sequence* current_seq = seq_init();
    struct trie* trie = create_trie();

    if (ins == NULL || outs == NULL || current_seq == NULL || trie == NULL) {
        free_structs(ins, outs, current_seq, trie);
        return false;
    }

    int next;
    code_t next_code = 0;
    unsigned int current_bits = start_bits;

    while ((next = ins_read_bits(ins, current_bits)) != EOF) {
        // TODO: generate sequence of codes and write them 1 byte at a time
        unsigned char const byte = next;
        outs_write_bits(outs, next_code, current_bits);
    }

    free_structs(ins, outs, current_seq, trie);
    return false;
}

/*
 * lzwDecode: Decode the bytes read via read_byte using LZW compression
 *            with variable-width codes, writing the result via write_byte.
 */
bool lzwDecode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void* context),
        void (*write_byte)(unsigned char c, void* context),
        void* context)
{
    if (!verify_params(start_bits, max_bits, read_byte, write_byte)) {
        return false;
    }

    struct instream* ins = ins_init(context, read_byte);
    struct outstream* outs = outs_init(context, write_byte);
    struct sequence* current_seq = seq_init();
    struct trie* trie = create_trie();

    if (ins == NULL || outs == NULL || current_seq == NULL || trie == NULL) {
        free_structs(ins, outs, current_seq, trie);
        return false;
    }

    code_t current_code;
    unsigned int current_bits = start_bits;

    while ((current_code = ins_read_bits(ins, current_bits)) != EOF) {
        // TODO: decode sequence of codes and write them 1 byte at a time
        unsigned char next_output = '\0';
        outs_write_bits(outs, next_output, CHAR_BIT);
    }

    free_structs(ins, outs, current_seq, trie);
    return false;
}
