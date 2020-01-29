#include "dict.h"

#include <stdlib.h>
#include <stdbool.h>

bool test_init(void) {
    struct dict* dict = dict_init(NULL);
    dict_destroy(dict);

    return true;
}

int main(void) {
    bool success = test_init();

    return success ?
        EXIT_SUCCESS :
        EXIT_FAILURE;
}
