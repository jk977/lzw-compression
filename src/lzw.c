#include "lzw.h"

#include "sequence.h"
#include "trie.h"
#include "outstream.h"
#include "instream.h"

#include "types.h"

#include <stdio.h>
#include <limits.h>

struct params {
    unsigned int current_bits;
    unsigned int max_bits;
    int (*read)(void* context);
    void (*write)(unsigned char c, void* context);
    void* context;
};

/*
 * verify_params: Ensure that the given parameters are valid.
 */
static bool verify_params(struct params const* p)
{
    return p->current_bits >= LZW_MINIMUM_BITS
        && p->max_bits <= LZW_MAXIMUM_BITS
        && p->read != NULL
        && p->write != NULL;
}

static struct trie* create_dict(void)
{
    struct trie* dict = trie_init(-1);

    for (size_t i = 0; i < CHILDREN_COUNT; ++i) {
        char key = (char) i;

        if (!trie_insert(dict, &key, 1, i)) {
            trie_destroy(dict);
            return NULL;
        }
    }

    return dict;
}

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
    struct params const p = {
        .current_bits = start_bits,
        .max_bits = max_bits,
        .read = read_byte,
        .write = write_byte,
        .context = context
    };

    if (!verify_params(&p)) {
        return false;
    }

    struct instream* ins = ins_init(context, read_byte);
    struct outstream* outs = outs_init(context, write_byte);

    struct sequence* current_seq = seq_init();
    struct trie* dict = create_dict();

    if (ins == NULL || outs == NULL || current_seq == NULL || dict == NULL) {
        free_structs(ins, outs, current_seq, dict);
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
    struct params const p = {
        .current_bits = start_bits,
        .max_bits = max_bits,
        .read = read_byte,
        .write = write_byte,
        .context = context
    };

    if (!verify_params(&p)) {
        return false;
    }

    struct instream* ins = ins_init(context, read_byte);
    struct outstream* outs = outs_init(context, write_byte);

    if (ins == NULL || outs == NULL) {
        free_structs(ins, outs, NULL, NULL);
        return false;
    }

    code_t current_code;
    unsigned int current_bits = start_bits;

    while ((current_code = ins_read_bits(ins, current_bits)) != EOF) {
        // TODO: decode sequence of codes and write them 1 byte at a time
        unsigned char next_output = '\0';
        outs_write_bits(outs, next_output, CHAR_BIT);
    }

    ins_destroy(ins);
    outs_destroy(outs);

    return false;
}
