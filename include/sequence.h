/*
 * sequence.h: A structure representing a dynamically-sized sequence
 *             of characters with various associated operations.
 */

#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <stdbool.h>
#include <sys/types.h>

struct sequence;

/*
 * Construction/destruction functions
 */
struct sequence* seq_init(size_t length);
struct sequence* seq_copy(struct sequence const* seq);
void seq_destroy(struct sequence* seq);

/*
 * Informational functions
 */
size_t seq_length(struct sequence const* seq);
int seq_get(struct sequence* seq, size_t index);
int seq_first(struct sequence* seq);
int seq_last(struct sequence* seq);

/*
 * Mutator functions
 */
bool seq_push(struct sequence* seq, char c);
int seq_pop(struct sequence* seq);
void seq_clear(struct sequence* seq);

/*
 * Utility functions
 */
char* seq_to_cstr(struct sequence* seq);
int seq_cmp(struct sequence const* lhs, struct sequence const* rhs);
bool seq_equals(struct sequence const* lhs, struct sequence const* rhs);

#endif // SEQUENCE_H_
