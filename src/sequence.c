#include "sequence.h"

#include <stdlib.h>
#include <string.h>

struct sequence {
    char entry;
    struct sequence* next;
};

/*
 * seq_init_with: Initialize sequence with the given entry.
 */
static struct sequence* seq_init_with(char entry)
{
    struct sequence* seq = malloc(sizeof(*seq));

    if (seq == NULL) {
        return NULL;
    }

    seq->entry = entry;
    seq->next = NULL;
    return seq;
}

/*
 * seq_init: Initialize an empty sequence.
 */
struct sequence* seq_init(void)
{
    return seq_init_with('\0');
}

/*
 * seq_destroy: Free the sequence created by seq_init().
 */
void seq_destroy(struct sequence* seq)
{
    if (seq == NULL) {
        return;
    }

    seq_destroy(seq->next);
    free(seq);
}

/*
 * seq_length: Get the length of the sequence, excluding the initial
 *             empty node. Returns -1 if the argument is NULL.
 */
ssize_t seq_length(struct sequence const* seq)
{
    if (seq == NULL) {
        return -1;
    }

    ssize_t length = 0;
    seq = seq->next;

    while (seq != NULL) {
        seq = seq->next;
        ++length;
    }

    return length;
}

/*
 * seq_add: Adds the given entry to the end of the sequence.
 *          Returns true on successful addition, otherwise false.
 */
bool seq_add(struct sequence* seq, char entry)
{
    if (seq == NULL) {
        return false;
    }

    while (seq->next != NULL) {
        seq = seq->next;
    }

    seq->next = seq_init_with(entry);
    return seq->next != NULL;
}

/*
 * seq_from_str: Converts the given string to a sequence of chars.
 */
struct sequence* seq_from_str(char const* str)
{
    struct sequence* seq = seq_init();

    if (seq == NULL) {
        return NULL;
    }

    while (*str != '\0') {
        if (!seq_add(seq, *str)) {
            seq_destroy(seq);
            return NULL;
        }

        ++str;
    }

    return seq;
}

/*
 * seq_to_str: Converts the sequence to a null-terminated string.
 *             The return value must be freed by the caller using free().
 */
char* seq_to_str(struct sequence const* seq)
{
    if (seq == NULL) {
        return NULL;
    }

    char* str = malloc(seq_length(seq) + 1);
    char* current_str = str;
    struct sequence const* current_seq = seq;

    while (current_seq != NULL) {
        *str = current_seq->entry;
        current_seq = current_seq->next;
        ++str;
    }

    *str = '\0';
    return str;
}

/*
 * seq_cmp: Same as strcmp() but with sequences.
 */
int seq_cmp(struct sequence const* lhs, struct sequence const* rhs)
{
    if (lhs == NULL || rhs == NULL) {
        return -1;
    }

    while (lhs != NULL && rhs != NULL) {
        if (lhs->entry < rhs->entry) {
            return -1;
        } else if (lhs->entry > rhs->entry) {
            return 1;
        }

        lhs = lhs->next;
        rhs = rhs->next;
    }

    if (lhs == NULL && rhs == NULL) {
        return 0;
    } else if (lhs == NULL) {
        return -1;
    } else {
        return 1;
    }
}

/*
 * seq_equals: Checks the equality of two sequences, returning true if equal
 *             and false otherwise.
 */
bool seq_equals(struct sequence const* lhs, struct sequence const* rhs)
{
    return seq_cmp(lhs, rhs) == 0;
}
