/*
 * lzw.h: An LZW compression API with variable-width codes.
 */

#ifndef LZW_H_
#define LZW_H_

#include <stdbool.h>

bool lzw_encode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void* context),
        void (*write_byte)(unsigned char c, void* context),
        void* context);

bool lzw_decode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void* context),
        void (*write_byte)(unsigned char c, void* context),
        void* context);

#endif // LZW_H_
