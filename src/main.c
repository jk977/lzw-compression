#include "lzw.h"

#include <stdio.h>
#include <stdlib.h>

static char const* inpath = "lzw_in.txt";
static char const* outpath = "lzw_out.txt";

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

int main(void) {
    FILE* streams[] = { fopen(inpath, "r"), fopen(outpath, "w") };
    puts("Calling lzwEncode()...");

    unsigned int start_bits = 8;
    unsigned int max_bits = 24;

    if (lzwEncode(start_bits, max_bits, read_byte, write_byte, streams)) {
        puts("Success!");
        return EXIT_SUCCESS;
    } else {
        puts("Failed.");
        return EXIT_FAILURE;
    }
}
