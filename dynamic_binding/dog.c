#include "dog.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


void dog_init(dog_t *c) {
    dog_init_named(c, DOG_DEFAULT_NAME);
}

void dog_init_named(dog_t *c, const char *name) {
    animal_set_name(&c->super, name);
    c->jump_height = rand();

}

void dog_voice(animal_t *obj) {
    printf("%s: Bark\n", obj->name_);
}

void dog_jump(dog_t *d) {
    printf("%s: jump to %f\n", d->super.name_, d->jump_height);
}

