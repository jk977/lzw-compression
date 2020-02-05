#include "outstream.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include <limits.h>
#include <string.h>

static char const* default_file = "foo.txt";

void write_file(unsigned char c, void* context)
{
    FILE* stream = context;
    fputc(c, stream);
}

void test_large(struct outstream* outs)
{
    uint16_t content[] = { 0b0111101110110111, 0b1100000011101111 };
    size_t const write_size = 16;

    // expected write: 10110111 01111011 11101111 11000000
    // least-significant byte is written first, then most significant byte
    for (size_t i = 0; i < sizeof(content) / sizeof(content[0]); ++i) {
        outs_write_bits(outs, content[i], write_size);
    }
}

void test_bytes(struct outstream* outs)
{
    char const* content = "foobarbaz\n";

    for (size_t i = 0; i < strlen(content); ++i) {
        outs_write_bits(outs, content[i], CHAR_BIT);
    }
}

void test_bits(struct outstream* outs)
{
    unsigned char content[] = { 'a', '\n' };
    size_t write_size = 2;

    for (size_t i = 0; i < sizeof(content) / sizeof(content[0]); ++i) {
        unsigned char current = content[i];
        size_t written = 0;

        while (written < CHAR_BIT) {
            size_t actual_size = write_size;

            if (written + write_size > CHAR_BIT) {
                actual_size = CHAR_BIT - written;
            }

            outs_write_bits(outs, current, actual_size);
            current <<= write_size;
            written += write_size;
        }

        ++write_size;
    }
}

static void test_file(char const* path,
        void (*tester)(struct outstream*))
{
    FILE* stream = fopen(path, "w");
    struct outstream* outs = outs_init(stream, write_file);

    if (stream == NULL || outs == NULL) {
        exit(EXIT_FAILURE);
    }

    tester(outs);

    outs_destroy(outs);
    fclose(stream);
}

int main(void)
{
    test_file("a.txt", test_large);
    test_file("b.txt", test_bytes);
    test_file("c.txt", test_bits);

    return EXIT_SUCCESS;
}
