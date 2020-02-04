#include "outstream.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <limits.h>
#include <string.h>

static char const* filename = "./foo.txt";

void write_file(unsigned char c, void* context)
{
    FILE* stream = context;
    fputc(c, stream);
}

void test_chars(FILE* stream) {
    char const* content = "foobarbaz\n";
    struct outstream* outs = outs_init(stream, write_file);

    for (size_t i = 0; i < strlen(content); ++i) {
        outs_write_bits(outs, content[i], CHAR_BIT);
    }

    outs_destroy(outs);
}

int main(void)
{
    FILE* stream = fopen(filename, "w");

    if (stream == NULL) {
        return EXIT_FAILURE;
    }

    test_chars(stream);
    return EXIT_SUCCESS;
}
