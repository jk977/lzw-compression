/*
 * sequence.h: A structure representing a dynamically-sized sequence
 *             of characters with various associated operations.
 */

#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <stdbool.h>
#include <sys/types.h>

struct sequence;

struct sequence* seq_init(size_t length);
struct sequence* seq_copy(struct sequence const* seq);
void seq_destroy(struct sequence* seq);

size_t seq_length(struct sequence const* seq);
int seq_get(struct sequence* seq, size_t index);
int seq_first(struct sequence* seq);
int seq_last(struct sequence* seq);

bool seq_push(struct sequence* seq, char c);
int seq_pop(struct sequence* seq);
void seq_clear(struct sequence* seq);

char* seq_to_cstr(struct sequence* seq);

#endif // SEQUENCE_H_
