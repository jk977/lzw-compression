#include "lzw.h"

#include <stdio.h>
#include <stdlib.h>

static char const* encode_inpath = "lzw_encode_in.txt";
static char const* encode_outpath = "lzw_encode_out.txt";

static char const* decode_inpath = "lzw_encode_out.txt";
static char const* decode_outpath = "lzw_decode_out.txt";

static unsigned int const start_bits = 8;
static unsigned int const max_bits = 24;

int read_byte(void* context)
{
    FILE** streams = context;
    return fgetc(streams[0]);
}

void write_byte(unsigned char byte, void* context)
{
    FILE** streams = context;
    fputc(byte, streams[1]);
}

static bool encode(void)
{
    printf("Calling lzw_encode() on %s (out: %s)...\n", encode_inpath, encode_outpath);

    FILE* streams[] = { fopen(encode_inpath, "r"), fopen(encode_outpath, "w") };
    bool success = lzw_encode(start_bits, max_bits, read_byte, write_byte, streams);

    fclose(streams[0]);
    fclose(streams[1]);
    puts("Done!");

    return success;
}

static bool decode(void)
{
    printf("Calling lzw_decode() on %s (out: %s)...\n", decode_inpath, decode_outpath);

    FILE* streams[] = { fopen(decode_inpath, "r"), fopen(decode_outpath, "w") };
    bool success = lzw_decode(start_bits, max_bits, read_byte, write_byte, streams);

    fclose(streams[0]);
    fclose(streams[1]);
    puts("Done!");

    return success;
}

int main(void) {
    bool success = encode() && decode();

    if (success) {
        puts("Success!");
        return EXIT_SUCCESS;
    } else {
        puts("Failed.");
        return EXIT_FAILURE;
    }
}
