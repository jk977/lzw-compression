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
    if (trie == NULL || key == NULL) {
        return NULL;
    } else if (key_length == 0) {
        return trie;
    }

    char index = key[0];
    struct trie* child = trie->children[index];

    if (trie->entry != index || child == NULL) {
        return NULL;
    } else {
        return get_subtrie_at(child, key + 1, key_length - 1);
    }
}

bool trie_insert(struct trie* trie, char const* key, size_t key_length, value_t value) {
    if (trie == NULL || key == NULL) {
        return false;
    }

    struct trie* target = get_subtrie_at(trie, key, key_length - 1);
    char new_entry = key[key_length - 1];

    if (target == NULL || target->children[new_entry] != NULL) {
        return false;
    }

    struct trie* result = trie_init();
    result->entry = new_entry;
    result->value = value;

    target->children[new_entry] = result;
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
