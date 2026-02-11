#include "animal.h"
#include <string.h>
#include <stdlib.h>

void animal_set_name(animal_t *a, const char *name) {
   // free previous name?
    a->name_ = strdup(name);
}

void animal_destroy(animal_t *a) {
    free(a->name_);

    free(a);
}
