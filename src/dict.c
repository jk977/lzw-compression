#include "dict.h"

#include <stdlib.h>

struct dict {
    hasher_t hasher;
    code_t* codes;
};

static hash_t default_hasher(char const* str) {
    /*
     * default hasher; a modified version of djb2 from 
     * http://www.cse.yorku.ca/~oz/hash.html
     *
     * this hash function should be functionally identical
     * to djb2, but less obfuscated since compilers have
     * improved at optimizations since djb2 was created.
     */

    hash_t hash = 5381;

    while (*str++ != '\0') {
        hash = 33 * hash + *str;
    }

    return hash;
}

struct dict* dict_init(hasher_t hasher) {
    /*
     * create a new dictionary with the given hash function.
     *
     * if hasher is NULL, the default hashing algorithm is used.
     */

    size_t const initial_size = 64;

    struct dict* dict = malloc(sizeof(*dict));
    code_t* codes = malloc(sizeof(code_t) * initial_size);

    if (dict == NULL || codes == NULL) {
        free(dict);
        free(codes);

        return NULL;
    }

    if (hasher != NULL) {
        dict->hasher = hasher;
    } else {
        dict->hasher = default_hasher;
    }

    dict->codes = codes;

    return dict;
}

void dict_destroy(struct dict* dict) {
    /*
     * deallocate the given dictionary.
     */

    free(dict->codes);
    free(dict);
}

bool dict_contains(struct dict* dict, char const* key) {
    /*
     * check if the key exists in the dictionary.
     */

    hash_t hash = (dict->hasher)(key);

    (void) dict;
    (void) key;
    (void) hash;

    return false;
}

void dict_set(struct dict* dict, char const* key, code_t code) {
    /*
     * assign the key to the given code in the dictionary.
     */

    hash_t hash = (dict->hasher)(key);

    (void) dict;
    (void) key;
    (void) code;
    (void) hash;
}

code_t dict_get(struct dict* dict, char const* key) {
    /*
     * get the assigned code for the given key in the dictionary.
     */

    hash_t hash = (dict->hasher)(key);

    (void) dict;
    (void) key;
    (void) hash;

    return -1;
}
