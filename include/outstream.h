#ifndef OUTSTREAM_H_
#define OUTSTREAM_H_

#include <stddef.h>
#include <stdint.h>

struct outstream;

struct outstream* outs_init(void* context,
        void (*write_byte)(unsigned char c, void* context));

void outs_destroy(struct outstream* outs);

void outs_write_bits(struct outstream* outs, uint32_t bits, size_t bit_count);
void outs_flush(struct outstream* outs);

#endif // OUTSTREAM_H_
