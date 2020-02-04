#include "lzw.h"

#include "outstream.h"
#include "instream.h"
#include "types.h"

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

    int byte = 0;
    char next_output = '\0';

    while ((byte = read_byte(context)) != EOF) {
        // TODO: generate sequence of codes and write them 1 byte at a time
        next_output = byte;
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

    int byte = 0;
    char next_output = '\0';

    while ((byte = read_byte(context)) != EOF) {
        // TODO: decode sequence of codes and write them 1 byte at a time
        next_output = byte;
        write_char(next_output, context);
    }

    return false;
}
