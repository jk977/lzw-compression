/*
 * instream.h: Input stream that reads a variable number of bits at a time.
 */

#ifndef INSTREAM_H_
#define INSTREAM_H_

#include <stddef.h>
#include <stdint.h>

struct instream;

struct instream* ins_init(void* context,
        int (*read_bits)(void* context));

void ins_destroy(struct instream* ins);

int32_t ins_read_bits(struct instream* ins, size_t bit_count);

#endif // INSTREAM_H_
