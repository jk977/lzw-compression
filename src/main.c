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

static bool encode(char const* in_path, char const* out_path)
{
    printf("Encoding %s to %s...\n", in_path, out_path);
    FILE* streams[] = { fopen(in_path, "r"), fopen(out_path, "w") };

    if (streams[0] == NULL || streams[1] == NULL) {
        if (streams[0] != NULL) {
            fclose(streams[0]);
        } else if (streams[1] != NULL) {
            fclose(streams[1]);
        }

        return false;
    }

    bool const success = lzw_encode(START_BITS, MAX_BITS, read_byte, write_byte,
                                    streams);

    fclose(streams[0]);
    fclose(streams[1]);
    puts("Done!");

    return success;
}

static bool decode(char const* in_path, char const* out_path)
{
    printf("Calling lzw_decode() on %s (out: %s)...\n",
           in_path, out_path);

    FILE* streams[] = { fopen(in_path, "r"), fopen(out_path, "w") };

    if (streams[0] == NULL || streams[1] == NULL) {
        if (streams[0] != NULL) {
            fclose(streams[0]);
        } else if (streams[1] != NULL) {
            fclose(streams[1]);
        }

        return false;
    }

    bool success = lzw_decode(START_BITS, MAX_BITS, read_byte, write_byte,
                              streams);

    fclose(streams[0]);
    fclose(streams[1]);
    puts("Done!");

    return success;
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
    bool success;

    switch (mode) {
    case ENCODE:
        success = encode(in_path, out_path);
        break;
    case DECODE:
        success = decode(in_path, out_path);
        break;
    }

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
