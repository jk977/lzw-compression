#ifndef LZW_H_
#define LZW_H_

#include <stdbool.h>

/* DO NOT MODIFY THIS INTERFACE */

/* limiting sizes of compression code bits */
#define LZW_MINIMUM_BITS 8
#define LZW_MAXIMUM_BITS 24

/* Introduction:
   ------------
    
    The interface to lzwEncode and lzwDecode utilize the
    following arguments in common:
 
    unsigned int start_bits
                The beginning width of symbol codes used in
                the compressed data. Must be >= 8 and <= 24.
 
    unsigned int max_bits
                The largest number of bits that symbol codes
                are permitted to use.  To successfully decode 
                encoded data, these two values (bits & max_bits)'
                must be the same values for decoding as the
                values used for encoding.  Otherwise, decoding 
                may fail. Must be >= 8 and <= 24 and >= bits.
 
    int (*read_byte)(void* context)
                A pointer to a function, which when passed
                a pointer to "context" (see last argument),
                will return the next byte of input data.
                If there are no further bytes of input data,
                a negative number must be returned. This
                function pointer must not be NULL.
 
    void (*write_char)(unsigned char c, void* context)
                A pointer to a function which writes the 
                character c to the output destination.
                The context pointer (see the last argument)
                is passed to write_char() to provide it
                information about where to send the character.  
                This function pointer must not be NULL.
 
    void* context
                A pointer to information about the I/O
                environment of the caller.  The pointer
                is provided as an argument to calls to 
                read_byte() and write_char() to give those 
                functions the information. This argument
                may be NULL, if a NULL argument is suitable
                for read_byte() and write_char(). */

/* lzwEncode
   ---------
 
   Read data using read_byte(), compress it and write the
   compressed data using write_char(). Returns true on 
   success, false otherwise.  Failures will be due to 
   invalid arguments. */

bool lzwEncode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void* context),
        void (*write_char)(unsigned char c, void* context),
        void* context);

/* lzwDecode
   ---------
 
   Read compressed data using read_byte(), decompress it and
   write the decompressed data using write_char(). If decompression 
   fails, a false result will be returned, otherwise, if it 
   succeeds, a true result will be returned.  Failures may be due 
   to attempting to decompress uncompressed data, or decompressing
   data that was compressed with different values for start_bits or
   max_bits. Also, a false result is returned for invalid
   arguments. */

bool lzwDecode(unsigned int start_bits, unsigned int max_bits,
        int (*read_byte)(void* context),
        void (*write_char)(unsigned char c, void* context),
        void* context);

#endif // LZW_H_
