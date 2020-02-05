#include "instream.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <limits.h>
#include <string.h>

static char const* filename = "foo.txt";

int read_file(void* context)
{
    FILE* stream = context;
    return fgetc(stream);
}

void test_large(struct instream* ins) {
    int32_t c;
    size_t const read_size = 16;

    while ((c = ins_read_bits(ins, read_size)) != EOF) {
        unsigned char const first = c >> 8;
        unsigned char const second = c;

        putchar(first);
        putchar(second);
    }
}

void test_bytes(struct instream* ins) {
    int32_t c;

    while ((c = ins_read_bits(ins, 8)) != EOF) {
        putchar(c);
    }
}

void test_small(struct instream* ins) {
    int32_t c;
    size_t count = 0;
    unsigned char current = 0;

    while ((c = ins_read_bits(ins, 4)) != EOF) {
        if (count % 2 == 0) {
            current = c << 4;
        } else {
            current |= c;
            putchar(current);
        }

        ++count;
    }
}

void test_bits(struct instream* ins) {
    int32_t c;
    size_t count = 0;
    unsigned char current = 0;

    while ((c = ins_read_bits(ins, 1)) != EOF) {
        unsigned int const current_bit = 7 - count % 8;
        current |= c << current_bit;

        if (current_bit == 0) {
            putchar(current);
            current = 0;
        }

        ++count;
    }
}

int main(void)
{
    FILE* stream = fopen(filename, "r");

    if (stream == NULL) {
        return EXIT_FAILURE;
    }

    struct instream* ins = ins_init(stream, read_file);

    test_bytes(ins);
    rewind(stream);
    fflush(stdout);

    test_large(ins);
    rewind(stream);
    fflush(stdout);

    test_small(ins);
    rewind(stream);
    fflush(stdout);

    test_bits(ins);
    rewind(stream);
    fflush(stdout);

    ins_destroy(ins);
    fclose(stream);

    return EXIT_SUCCESS;
}
