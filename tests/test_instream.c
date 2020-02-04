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

void test_chars(struct instream* ins) {
    int32_t c;

    while ((c = ins_read_bits(ins, 8)) != EOF) {
        putchar(c);
    }
}

void test_bits(struct instream* ins) {
    int32_t c;
    size_t const read_size = 16;

    while ((c = ins_read_bits(ins, read_size)) != EOF) {
        for (size_t i = 0; i <= read_size; i += CHAR_BIT) {
            unsigned char const current = c >> (read_size - i);
            putchar(current);
        }
    }
}

int main(void)
{
    FILE* stream = fopen(filename, "r");

    if (stream == NULL) {
        return EXIT_FAILURE;
    }

    struct instream* ins = ins_init(stream, read_file);

    test_bits(ins);

    ins_destroy(ins);
    fclose(stream);

    return EXIT_SUCCESS;
}