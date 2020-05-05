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
 * seq_copy: Create a duplicate of the given sequence.
 */

struct sequence* seq_copy(struct sequence const* seq)
{
    struct sequence* copy = seq_init(seq->used);
    char* content_copy = malloc(seq->used);

    if (copy == NULL || content_copy == NULL) {
        free(content_copy);
        free(copy);

        return NULL;
    }

    // only copy the meaningful characters, ignoring the garbage
    memcpy(content_copy, seq->content, seq->used);

    copy->content = content_copy;
    copy->used = seq->used;

    return copy;
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
 * seq_length: Get the length of the sequence, excluding the null byte.
 */

size_t seq_length(struct sequence const* seq)
{
    return seq->used;
}

/*
 * seq_get: Gets the character at the given index, or -1 if index is invalid.
 */

int seq_get(struct sequence* seq, size_t index)
{
    if (seq->used <= index) {
        return -1;
    }

    return seq->content[index];
}

/*
 * seq_first: Gets the first element of the sequence,
 *            or -1 if the sequence is empty.
 */

int seq_first(struct sequence* seq)
{
    if (seq->used == 0) {
        return -1;
    }

    return seq_get(seq, 0);
}

/*
 * seq_last: Gets the last element of the sequence,
 *           or -1 if the sequence is empty.
 */

int seq_last(struct sequence* seq)
{
    if (seq->used == 0) {
        return -1;
    }

    return seq_get(seq, seq->used - 1);
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
        // not enough space, so double the buffer size first
        size_t const new_length = seq->length * 2;
        char* new_content = realloc(seq->content, new_length);

        if (new_content == NULL) {
            return false;
        }

        seq->content = new_content;
        seq->length = new_length;
    }

    seq->content[seq->used] = c;
    ++seq->used;

    return true;
}

/*
 * seq_pop: Shrinks the sequence by 1 and returns the element
 *          that was removed, or -1 if sequence is empty.
 */

int seq_pop(struct sequence* seq)
{
    if (seq->used == 0) {
        return -1;
    }

    char result = seq_last(seq);
    --seq->used;

    return result;
}

/*
 * seq_clear: Clear the sequence but keep any previously allocated memory.
 */

void seq_clear(struct sequence* seq)
{
    seq->used = 0;
}

/*
 * seq_as_cstr: Converts sequence to C-string. The result must be freed by
 *              the caller with free().
 */

char* seq_to_cstr(struct sequence* seq)
{
    char* result = malloc(seq->used + 1);

    if (result == NULL) {
        return NULL;
    }

    strncpy(result, seq->content, seq->used);
    result[seq->used] = '\0';

    return result;
}
