#include "lzw.h"
#include "types.h"
#include "trie.h"

struct params {
    unsigned int current_bits;
    unsigned int max_bits;
    int (*read)(void* context);
    void (*write)(unsigned char c, void* context);
    void* context;
};

static bool verify_params(struct params* p) {
    return p->current_bits >= LZW_MINIMUM_BITS
        && p->max_bits <= LZW_MAXIMUM_BITS
        && p->read != NULL
        && p->write != NULL;
}

bool lzwEncode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void* context),
        void (*write_char)(unsigned char c, void* context),
        void* context)
{
    struct params p = {
        .current_bits = start_bits,
        .max_bits = max_bits,
        .read = read_byte,
        .write = write_char,
        .context = context
    };

    if (!verify_params(&p)) {
        return false;
    }

    int byte;
    char next_output;

    while ((byte = read_byte(context)) != EOF) {
        code_t code;
        // TODO: get sequence of codes and write them, 1 char at a time
        write_char(next_output, context);
    }

    return false;
}

bool lzwDecode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void* context),
        void (*write_char)(unsigned char c, void* context),
        void* context)
{
    struct params p = {
        .current_bits = start_bits,
        .max_bits = max_bits,
        .read = read_byte,
        .write = write_char,
        .context = context
    };

    if (!verify_params(&p)) {
        return false;
    }

    return false;
}
