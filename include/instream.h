#ifndef INSTREAM_H_
#define INSTREAM_H_

struct instream;

struct instream* ins_init(int (*read_bits)(void* context));
void ins_destroy(struct instream* ins);

long int ins_read_bits(struct instream* ins, size_t bit_count);
long int ins_flush(struct instream* ins);

#endif // INSTREAM_H_
