#include "outstream.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include <limits.h>
#include <string.h>

#define FOREACH(i, ary) \
    for (size_t i = 0; i < sizeof(ary) / sizeof((ary)[0]); ++i)

static char const* default_file = "foo.txt";

void write_file(unsigned char c, void* context)
{
    FILE* stream = context;
    fputc(c, stream);
}

void test_large(struct outstream* outs)
{
    uint16_t content[] = { 0b1011011101111011, 0b1110111111000000 };
    size_t const write_size = 16;

    // expected write: 10110111 01111011 11101111 11000000
    FOREACH (i, content) {
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

    FOREACH(i, content) {
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

void test_vary(struct outstream* outs)
{
    unsigned char content[] = {
        0b1, 0b10, 0b100, 0b1000, 0b10000, 0b100000
    };

    size_t write_size = 1;

    // expected write: 11010010 00100001 00000000
    FOREACH(i, content) {
        outs_write_bits(outs, content[i], write_size);
        ++write_size;
    }

    outs_flush(outs);
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
    test_file("d.txt", test_vary);

    return EXIT_SUCCESS;
}
