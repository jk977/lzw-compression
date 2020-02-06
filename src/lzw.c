#include "lzw.h"

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

static bool verify_params(struct params const* p)
{
    return p->current_bits >= LZW_MINIMUM_BITS
        && p->max_bits <= LZW_MAXIMUM_BITS
        && p->read != NULL
        && p->write != NULL;
}

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
    struct trie* trie = trie_init(-1);

    int next;
    code_t next_code = 0;
    unsigned int current_bits = start_bits;

    while ((next = ins_read_bits(ins, current_bits)) != EOF) {
        // TODO: generate sequence of codes and write them 1 byte at a time
        unsigned char const byte = next;
        (void) byte;
        outs_write_bits(outs, next_code, current_bits);
    }

    return false;
}

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
    struct trie* trie = trie_init(-1);

    code_t code;
    unsigned int current_bits = start_bits;

    while ((code = ins_read_bits(ins, current_bits)) != EOF) {
        // TODO: decode sequence of codes and write them 1 byte at a time
        unsigned char next_output = '\0';
        outs_write_bits(outs, next_output, CHAR_BIT);
    }

    return false;
}
