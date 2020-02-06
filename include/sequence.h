#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <stdbool.h>
#include <sys/types.h>

struct sequence;

struct sequence* seq_init(size_t length);
void seq_destroy(struct sequence* seq);

char* seq_as_cstr(struct sequence* seq);

size_t seq_length(struct sequence const* seq);
int seq_get(struct sequence* seq, size_t i);
int seq_last(struct sequence* seq);

bool seq_push(struct sequence* seq, char c);
int seq_pop(struct sequence* seq);

int seq_cmp(struct sequence const* lhs, struct sequence const* rhs);
bool seq_equals(struct sequence const* lhs, struct sequence const* rhs);

#endif // SEQUENCE_H_
