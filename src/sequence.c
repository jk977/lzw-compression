#include "sequence.h"

#include <stdlib.h>
#include <string.h>

struct sequence {
    char* content;
    size_t length;
    size_t used;
};

/*
 * seq_init: Initialize an empty sequence.
 */
struct sequence* seq_init(size_t length)
{
    if (length == 0) {
        // don't accept 0-length sequences
        return NULL;
    }

    struct sequence* seq = malloc(sizeof(*seq));
    char* content = malloc(length);

    if (seq == NULL || content == NULL) {
        free(content);
        free(seq);

        return NULL;
    }

    seq->content = content;
    seq->length = length;
    seq->used = 0;

    return seq;
}

/*
 * seq_destroy: Free the sequence created by seq_init().
 */
void seq_destroy(struct sequence* seq)
{
    if (seq == NULL) {
        return;
    }

    free(seq->content);
    free(seq);
}

/*
 * seq_length: Get the length of the sequence, excluding the initial
 *             empty node. Returns -1 if the argument is NULL.
 */
size_t seq_length(struct sequence const* seq)
{
    return seq->length;
}

/*
 * seq_push: Adds the given entry to the end of the sequence.
 *           Returns true on successful addition, otherwise false.
 */
bool seq_push(struct sequence* seq, char c)
{
    if (seq == NULL) {
        return false;
    }

    if (seq->used == seq->length) {
        char* new_content = realloc(seq->content, seq->length * 2);

        if (new_content == NULL) {
            return false;
        }

        seq->content = new_content;
        seq->length *= 2;
    }

    seq->content[seq->used] = c;
    ++seq->used;

    return true;
}

/*
 * seq_cmp: Functionally identical to strcmp().
 */
int seq_cmp(struct sequence const* lhs, struct sequence const* rhs)
{
    size_t smallest_size = (lhs->used > rhs->used) ?
        rhs->used :
        lhs->used;

    return strncmp(lhs->content, rhs->content, smallest_size);
}

/*
 * seq_equals: Checks the equality of two sequences, returning true if equal
 *             and false otherwise.
 */
bool seq_equals(struct sequence const* lhs, struct sequence const* rhs)
{
    return seq_cmp(lhs, rhs) == 0;
}
