#ifndef DICT_H_
#define DICT_H_

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t hash_t;
typedef uint32_t code_t;

typedef hash_t (*hasher_t)(char const*);

struct dict;

struct dict* dict_init(hasher_t hasher);
void dict_destroy(struct dict* dict);

bool dict_contains(struct dict* dict, char const* key);

void dict_set(struct dict* dict, char const* key, code_t code);
code_t dict_get(struct dict* dict, char const* key);

#endif // DICT_H_
