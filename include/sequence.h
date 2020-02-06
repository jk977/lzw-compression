#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <stdbool.h>
#include <sys/types.h>

struct sequence;

struct sequence* seq_init(size_t length);
void seq_destroy(struct sequence* seq);

size_t seq_length(struct sequence const* seq);
bool seq_append(struct sequence* seq, char c);

int seq_cmp(struct sequence const* lhs, struct sequence const* rhs);
bool seq_equals(struct sequence const* lhs, struct sequence const* rhs);

#endif // SEQUENCE_H_
