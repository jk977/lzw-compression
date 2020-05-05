#include "lzw.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#define INIT_BITS   8
#define MAX_BITS    24

static char const* program_name;

static void usage(FILE* stream)
{
    fprintf(stream, "Usage:\n");
    fprintf(stream, "\t%s (-d | -e)\n", program_name);
    fprintf(stream, "\n");

    fprintf(stream, "Options:\n");
    fprintf(stream, "\t-d\tDecode IN_PATH and store the result in OUT_PATH\n");
    fprintf(stream, "\t-e\tEncode IN_PATH and store the result in OUT_PATH\n");
    fprintf(stream, "\n");

    fprintf(stream, "Encode or decode the bytes read from stdin using LZW\n");
    fprintf(stream, "compression and print the result to stdout.\n");
}

static int read_byte(void* ctx)
{
    return fgetc(ctx);
}

static void print_byte(unsigned char byte, void* ctx)
{
    (void) ctx;
    putchar(byte);
}

int main(int argc, char** argv) {
    program_name = argv[0];

    if (argc != 2) {
        usage(stderr);
        return EXIT_FAILURE;
    }

    enum { ENCODE, DECODE } mode = ENCODE;
    int opt;

    while ((opt = getopt(argc, argv, "deh")) != -1) {
        switch (opt) {
        case 'd':
            mode = DECODE;
            break;
        case 'e':
            mode = ENCODE;
            break;
        case 'h':
            usage(stdout);
            return EXIT_SUCCESS;
        default:
            usage(stderr);
            return EXIT_FAILURE;
        }
    }

    bool success;

    switch (mode) {
    case ENCODE:
        success = lzw_encode(INIT_BITS, MAX_BITS, read_byte, print_byte, stdin);
        break;
    case DECODE:
        success = lzw_decode(INIT_BITS, MAX_BITS, read_byte, print_byte, stdin);
        break;
    }

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
