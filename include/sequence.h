#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <stdbool.h>
#include <stddef.h>

typedef char seqval_t;
struct sequence;

struct sequence* seq_init(void);
void seq_destroy(struct sequence* seq);

size_t seq_length(struct sequence const* seq);
bool seq_add(struct sequence* seq, char entry);

struct sequence* seq_from_str(char const* str);
char* seq_to_str(struct sequence const* seq);

int seq_cmp(struct sequence const* lhs, struct sequence const* rhs);
bool seq_equals(struct sequence const* lhs, struct sequence const* rhs);

#endif // SEQUENCE_H_
