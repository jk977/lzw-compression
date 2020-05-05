#include "lzw.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#define START_BITS  8
#define MAX_BITS    24

static char const* program_name;

static void usage(FILE* stream)
{
    fprintf(stream, "usage: %s -d IN_PATH OUT_PATH\n", program_name);
    fprintf(stream, "usage: %s -e IN_PATH OUT_PATH\n\n", program_name);

    fprintf(stream, "Options:\n");
    fprintf(stream, "\t-d\tDecode IN_PATH and store the result in OUT_PATH\n");
    fprintf(stream, "\t-e\tEncode IN_PATH and store the result in OUT_PATH\n");
}

static int read_byte(void* context)
{
    FILE** streams = context;
    return fgetc(streams[0]);
}

static void write_byte(unsigned char byte, void* context)
{
    FILE** streams = context;
    fputc(byte, streams[1]);
}

static FILE* fopen_or_fail(char const* pathname, char const* mode)
{
    FILE* stream = fopen(pathname, mode);

    if (stream == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    return stream;
}

int main(int argc, char** argv) {
    program_name = argv[0];

    if (argc != 4) {
        usage(stderr);
        return EXIT_FAILURE;
    }

    enum { ENCODE, DECODE } mode = ENCODE;
    int opt;

    while ((opt = getopt(argc, argv, "de")) != -1) {
        switch (opt) {
        case 'd':
            mode = DECODE;
            break;
        case 'e':
            mode = ENCODE;
            break;
        default:
            usage(stderr);
            return EXIT_FAILURE;
        }
    }

    if (optind != 2) {
        usage(stderr);
        return EXIT_FAILURE;
    }

    char const* in_path = argv[optind];
    char const* out_path = argv[optind + 1];

    FILE* in_stream = fopen_or_fail(in_path, "r");
    FILE* out_stream = fopen_or_fail(out_path, "w");
    FILE* streams[] = { in_stream, out_stream };

    bool success;

    switch (mode) {
    case ENCODE:
        success = lzw_encode(START_BITS, MAX_BITS, read_byte, write_byte,
                             streams);
        break;
    case DECODE:
        success = lzw_decode(START_BITS, MAX_BITS, read_byte, write_byte,
                             streams);
        break;
    }

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
