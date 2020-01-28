#ifndef DICT_H
#define DICT_H
#include <stdbool.h>

#include "sequence.h"

typedef struct _dict {
    int placeHolder;    // replace this line with your desired contents
} Dict;

Dict* newDict(unsigned int hashSize);

void deleteDictDeep(Dict* dict);

bool searchDict(Dict* dict, Sequence* key, unsigned int* code);

void insertDict(Dict* dict, Sequence* key, unsigned int  code);

#endif
