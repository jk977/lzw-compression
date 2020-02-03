#include "trie.h"

#include <stdlib.h>
#include <stdbool.h>

#include <assert.h>

void test_init(void) {
    struct trie* trie = trie_init();
    assert(trie != NULL);
    trie_destroy(trie);
}

void test_insert(void) {
    struct trie* trie = trie_init();

    assert(trie != NULL);

    assert( trie_cstr_insert(trie, "f", 1) );
    assert( trie_cstr_insert(trie, "fo", 2) );
    assert( trie_cstr_insert(trie, "foo", 3) );

    assert( trie_cstr_insert(trie, "b", 4) );
    assert( trie_cstr_insert(trie, "ba", 5) );
    assert( trie_cstr_insert(trie, "bar", 6) );

    assert( !trie_cstr_insert(trie, "b", 7) );
    assert( !trie_cstr_insert(trie, "ba", 8) );
    assert( trie_cstr_insert(trie, "baz", 9) );

    trie_destroy(trie);
}

void test_contains(void) {
    struct trie* trie = trie_init();

    trie_cstr_insert(trie, "f", 1);
    trie_cstr_insert(trie, "fo", 2);
    trie_cstr_insert(trie, "foo", 3);

    assert( trie_cstr_contains(trie, "f") );
    assert( trie_cstr_contains(trie, "fo") );
    assert( trie_cstr_contains(trie, "foo") );

    trie_cstr_insert(trie, "b", 4);
    trie_cstr_insert(trie, "ba", 5);
    trie_cstr_insert(trie, "bar", 6);

    assert( trie_cstr_contains(trie, "b") );
    assert( trie_cstr_contains(trie, "ba") );
    assert( trie_cstr_contains(trie, "bar") );

    trie_cstr_insert(trie, "baz", 7);

    assert( trie_cstr_contains(trie, "baz") );

    trie_destroy(trie);
}

int main(void) {
    test_init();
    test_insert();
    test_contains();

    return EXIT_SUCCESS;
}
