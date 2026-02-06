#include "cat.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

cat_t cat_create() {
    return cat_create_named(CAT_DEFAULT_NAME);
}

cat_t cat_create_named(const char *name) {
    size_t name_len = strlen(name);
    char * name_mem = (char * ) calloc(name_len+1, 1);
    assert(name_mem);

    strncpy(name_mem, name, name_len);
    cat_t c = {name_mem};
    return c;
}

void cat_destroy(cat_t *obj) {
    free(obj->name_);
}

void cat_voice(cat_t *obj) {
    printf("%s: Meow\n", obj->name_);
}
