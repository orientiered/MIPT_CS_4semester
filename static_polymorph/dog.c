#include "dog.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

dog_t *dog_create() {
    return dog_create_named(DOG_DEFAULT_NAME);
}

dog_t *dog_create_named(const char *name) {
    dog_t *temp = calloc(1, sizeof(dog_t));

    animal_set_name(&temp->super, name);
    temp->super.voice = dog_voice;
    temp->super.destroy = dog_destroy;

    temp->jump_height = rand();
    return temp;
}

void dog_destroy(animal_t *obj) {
    animal_destroy(obj);
}

void dog_voice(animal_t *obj) {
    printf("%s: Bark\n", obj->name_);
}

void dog_jump(dog_t *d) {
    printf("%s: jump to %f\n", d->super.name_, d->jump_height);
}

