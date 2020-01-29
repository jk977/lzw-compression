#include "dict.h"

#include <stdlib.h>
#include <stdbool.h>

bool test_init(void) {
    struct dict* dict = dict_init(NULL);
    dict_destroy(dict);

    return true;
}

bool test_set(void) {
    struct dict* dict = dict_init(NULL);

    dict_set(dict, "foo", 1);
    dict_set(dict, "bar", 2);
    dict_set(dict, "baz", 3);

    dict_destroy(dict);

    return true;
}

bool test_contains(void) {
    bool success = true;
    struct dict* dict = dict_init(NULL);

    dict_set(dict, "foo", 1);
    dict_set(dict, "bar", 2);
    dict_set(dict, "baz", 3);

    success = success
        && dict_contains(dict, "foo")
        && dict_contains(dict, "bar")
        && dict_contains(dict, "baz");

    dict_destroy(dict);

    return success;
}

int main(void) {
    bool success = test_init()
        && test_set()
        && test_contains();

    return success ?
        EXIT_SUCCESS :
        EXIT_FAILURE;
}
