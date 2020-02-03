#include "trie.h"

#include <stdlib.h>
#include <string.h>

#define CHILDREN_COUNT 256

struct trie {
    char entry;
    value_t value;
    struct trie* children[CHILDREN_COUNT];
};

struct trie* trie_init(void) {
    /*
     * create an empty trie.
     */

    struct trie* trie = malloc(sizeof *trie);

    if (trie == NULL) {
        return NULL;
    }

    trie->entry = '\0';
    trie->value = 0;

    for (size_t i = 0; i < CHILDREN_COUNT; ++i) {
        trie->children[i] = NULL;
    }

    return trie;
}

void trie_destroy(struct trie* trie) {
    /*
     * free the allocated trie.
     */

    if (trie == NULL) {
        return;
    }

    for (size_t i = 0; i < CHILDREN_COUNT; ++i) {
        trie_destroy(trie->children[i]);
    }

    free(trie);
}

static struct trie* get_subtrie_at(struct trie* trie, char const* key, size_t key_length) {
    /*
     * find the subtrie with the given key. for instance, if key is "abc",
     * the trie will be traversed from the root node to the child at 'a', then
     * 'b', then 'c', and return a pointer to the resulting subtrie.
     */

    if (trie == NULL || key == NULL) {
        return NULL;
    }
    
    if (key_length == 0) {
        // length being 0 implies that the current trie is the result
        return trie;
    }

    char index = key[0];
    struct trie* child = trie->children[index];
    bool trie_matches_key = (index == trie->entry);

    if (!trie_matches_key || child == NULL) {
        // no such key exists in trie
        return NULL;
    } else {
        // descend to next subtrie
        char const* tail = key + 1;
        size_t const tail_length = key_length - 1;

        return get_subtrie_at(child, tail, tail_length);
    }
}

bool trie_insert(struct trie* trie, char const* key, size_t key_length, value_t value) {
    if (trie == NULL || key == NULL) {
        return false;
    }

    size_t const tail_length = key_length - 1;
    struct trie* target = get_subtrie_at(trie, key, tail_length);
    char new_entry = key[tail_length];

    if (target == NULL || target->children[new_entry] != NULL) {
        // expect target to be non-null, with a null child where the new entry is
        return false;
    }

    struct trie* result = trie_init();

    if (result == NULL) {
        return false;
    }

    target->children[new_entry] = result;
    result->entry = new_entry;
    result->value = value;

    return true;
}

bool trie_contains(struct trie* trie, char const* key, size_t key_length) {
    return trie_lookup(trie, key, key_length) != NULL;
}

value_t* trie_lookup(struct trie* trie, char const* key, size_t key_length) {
    struct trie* result = get_subtrie_at(trie, key, key_length);

    return (result != NULL) ?
        &result->entry :
        NULL;
}

value_t* trie_cstr_insert(struct trie* trie, char const* key, value_t value) {
    return trie_lookup(trie, key, strlen(key), value);
}

value_t* trie_cstr_lookup(struct trie* trie, char const* key) {
    return trie_lookup(trie, key, strlen(key));
}

bool trie_cstr_contains(struct trie* trie, char const* key) {
    return trie_contains(trie, key, strlen(key));
}
