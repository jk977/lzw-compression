#include "dict.h"

#include <stdlib.h>

struct dict {
    hasher_t hasher;
    code_t last_code;
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
    size_t const initial_size = 64;

    struct dict* dict = malloc(sizeof *dict);
    code_t* codes = malloc(sizeof code_t * initial_size);

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

    dict->last_code = 0;
    dict->codes = codes;

    return dict;
}

void dict_destroy(struct dict* dict) {
    free(dict->codes);
    free(dict);
}

bool dict_contains(struct dict* dict, char const* key, code_t code) {
    return false;
}

void dict_set(struct dict* dict, char const* key, code_t code) {
}

code_t dict_get(struct dict* dict, char const* key) {
    return -1;
}
