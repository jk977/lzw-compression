#ifndef LZW_H_
#define LZW_H_

#include <stdbool.h>

#define LZW_MINIMUM_BITS 8
#define LZW_MAXIMUM_BITS 24

bool lzwEncode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void* context),
        void (*write_byte)(unsigned char c, void* context),
        void* context);

bool lzwDecode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void* context),
        void (*write_byte)(unsigned char c, void* context),
        void* context);

#endif // LZW_H_
