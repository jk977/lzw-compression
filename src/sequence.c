#include "sequence.h"

#include <stdlib.h>
#include <string.h>

struct sequence {
    char entry;
    struct sequence* next;
};

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

struct sequence* seq_init(void)
{
    return seq_init_with('\0');
}

void seq_destroy(struct sequence* seq)
{
    if (seq == NULL) {
        return;
    }

    seq_destroy(seq->next);
    free(seq);
}

size_t seq_length(struct sequence const* seq)
{
    size_t length = 0;

    while (seq != NULL) {
        seq = seq->next;
        ++length;
    }

    return length;
}

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

bool seq_equals(struct sequence const* lhs, struct sequence const* rhs)
{
    return seq_cmp(lhs, rhs) == 0;
}
