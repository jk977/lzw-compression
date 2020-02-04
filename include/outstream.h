#ifndef OUTSTREAM_H_
#define OUTSTREAM_H_

struct outstream;

struct outstream* outs_init(void (*write_byte)(unsigned char c, void* context));
void outs_destroy(struct outstream* outs);

void outs_write_bits(struct outstream* outs, int bits, size_t bit_count);
void outs_flush(struct outstream* outs);

#endif // OUTSTREAM_H_
