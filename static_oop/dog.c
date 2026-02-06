#include "dog.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

dog_t dog_create() {
    return dog_create_named(DOG_DEFAULT_NAME);
}

dog_t dog_create_named(const char *name) {
    size_t name_len = strlen(name);
    char * name_mem = (char * ) calloc(name_len+1, 1);
    assert(name_mem);

    strncpy(name_mem, name, name_len);
    dog_t c = {name_mem};
    return c;
}

void dog_destroy(dog_t *obj) {
    free(obj->name_);
}

void dog_voice(dog_t *obj) {
    printf("%s: Bark\n", obj->name_);
}
