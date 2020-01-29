#include "dict.h"

#include <stdlib.h>

struct dict {
    hasher_t hasher;
    code_t last_code;
    code_t* codes;
};

struct dict* dict_init(hasher_t hasher) {
    size_t const initial_size = 50;

    struct dict* dict = malloc(sizeof *dict);
    code_t* codes = malloc(sizeof code_t * initial_size);

    if (dict == NULL || codes == NULL) {
        free(dict);
        free(codes);

        return NULL;
    }

    dict->hasher = hasher;
    dict->last_code = 0;
    dict->codes = codes;

    return dict;
}

void dict_destroy(struct dict* dict) {
    free(dict->codes);
    free(dict);
}

bool dict_contains(struct dict* dict, char const* key, code_t code) {
}

void dict_set(struct dict* dict, char const* key, code_t code) {
}

code_t dict_get(struct dict* dict, char const* key) {
}
