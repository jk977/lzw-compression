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

    if (lzwEncode(16, 16, read_byte, write_byte, streams)) {
        puts("Success!");
        return EXIT_SUCCESS;
    } else {
        puts("Failed.");
        return EXIT_FAILURE;
    }
}
