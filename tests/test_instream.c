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

void test_odd(struct instream* ins) {
    int32_t c;
    size_t count = 0;

    while ((c = ins_read_bits(ins, 3)) != EOF) {
        printf("%d ", c);
        ++count;
    }

    puts("");
}

void test_varying(struct instream* ins) {
    int32_t c;
    size_t read_size = 1;

    while ((c = ins_read_bits(ins, read_size)) != EOF) {
        printf("%d ", c);
        ++read_size;
    }

    puts("");
}

int main(void)
{
    FILE* large_stream = fopen(filename, "r");
    FILE* bytes_stream = fopen(filename, "r");
    FILE* small_stream = fopen(filename, "r");
    FILE* bits_stream = fopen(filename, "r");
    FILE* odd_stream = fopen("odd.txt", "r");
    FILE* vary_stream = fopen("odd.txt", "r");

    if (large_stream == NULL
            || bytes_stream == NULL
            || small_stream == NULL
            || bits_stream == NULL
            || odd_stream == NULL
            || vary_stream == NULL) {
        return EXIT_FAILURE;
    }

    struct instream* bytes_ins = ins_init(bytes_stream, read_file);
    struct instream* large_ins = ins_init(large_stream, read_file);
    struct instream* small_ins = ins_init(small_stream, read_file);
    struct instream* bits_ins = ins_init(bits_stream, read_file);
    struct instream* odd_ins = ins_init(odd_stream, read_file);
    struct instream* vary_ins = ins_init(vary_stream, read_file);

    test_large(large_ins);
    test_bytes(bytes_ins);
    test_small(small_ins);
    test_bits(bits_ins);
    test_odd(odd_ins);
    test_odd(vary_ins);

    ins_destroy(vary_ins);
    ins_destroy(odd_ins);
    ins_destroy(bits_ins);
    ins_destroy(small_ins);
    ins_destroy(bytes_ins);
    ins_destroy(large_ins);

    fclose(vary_stream);
    fclose(odd_stream);
    fclose(bits_stream);
    fclose(small_stream);
    fclose(bytes_stream);
    fclose(large_stream);

    return EXIT_SUCCESS;
}
