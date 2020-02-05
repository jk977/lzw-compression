#include "instream.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <limits.h>
#include <string.h>

static char const* default_file = "default-instream.txt";

static int read_file(void* context)
{
    FILE* stream = context;
    return fgetc(stream);
}

static void test_large(struct instream* ins) {
    int32_t c;
    size_t const read_size = 16;

    while ((c = ins_read_bits(ins, read_size)) != EOF) {
        unsigned char const first = c >> 8;
        unsigned char const second = c;

        putchar(first);
        putchar(second);
    }
}

static void test_bytes(struct instream* ins) {
    int32_t c;

    while ((c = ins_read_bits(ins, 8)) != EOF) {
        putchar(c);
    }
}

static void test_small(struct instream* ins) {
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

static void test_bits(struct instream* ins) {
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

static void test_odd(struct instream* ins) {
    int32_t c;
    size_t count = 0;

    while ((c = ins_read_bits(ins, 3)) != EOF) {
        printf("%d ", c);
        ++count;
    }

    puts("");
}

static void test_vary(struct instream* ins) {
    int32_t c;
    size_t read_size = 1;

    while ((c = ins_read_bits(ins, read_size)) != EOF) {
        printf("%d ", c);
        ++read_size;
    }

    puts("");
}

static void test_file(char const* path,
        void (*tester)(struct instream*))
{
    FILE* stream = fopen(path, "r");
    struct instream* ins = ins_init(stream, read_file);

    if (stream == NULL || ins == NULL) {
        exit(EXIT_FAILURE);
    }

    tester(ins);
    ins_destroy(ins);
    fclose(stream);
}

int main(void)
{
    test_file(default_file, test_large);
    test_file(default_file, test_bytes);
    test_file(default_file, test_small);
    test_file(default_file, test_bits);

    test_file("odd.txt", test_odd);
    test_file("vary.txt", test_vary);

    return EXIT_SUCCESS;
}
