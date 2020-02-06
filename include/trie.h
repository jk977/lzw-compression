#ifndef TRIE_H_
#define TRIE_H_

#include <stddef.h>
#include <stdbool.h>

#include "types.h"

typedef code_t value_t;

struct trie;

/*
 * trie construction and destruction methods.
 */
struct trie* trie_init(value_t value);
void trie_destroy(struct trie* trie);

/*
 * trie operations:
 *  - insert() puts the key-value pair into the trie, returning
 *      false if the prefix isn't present in the trie or allocation fails.
 *  - lookup() returns the value corresponding to the given key, or
 *      NULL if not found.
 *  - contains() returns true if the key is in the trie, otherwise false.
 */
bool trie_insert(struct trie* trie, char const* key, size_t key_length, value_t value);
value_t* trie_lookup(struct trie* trie, char const* key, size_t key_length);
bool trie_contains(struct trie* trie, char const* key, size_t key_length);

/*
 * C-string operations:
 * These are equivalent to the previous trie operations, but assume that
 * key is null-terminated to eliminate the need to pass the key size.
 */
bool trie_cstr_insert(struct trie* trie, char const* key, value_t value);
value_t* trie_cstr_lookup(struct trie* trie, char const* key);
bool trie_cstr_contains(struct trie* trie, char const* key);

#endif // TRIE_H_
