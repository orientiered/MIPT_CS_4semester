#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "cat.h"

cat_t* cat_create() {
    return cat_create_named(CAT_DEFAULT_NAME);
}

cat_t* cat_create_named(const char *name) {
    cat_t *temp = calloc(1, sizeof(cat_t));

    animal_set_name(&temp->super, name);
    temp->super.voice = cat_voice;
    temp->super.destroy = cat_destroy;

    temp->claw_length = rand();
    return temp;
}

void cat_destroy(animal_t *obj) {
    // child class destroy
    cat_t *c = (cat_t *) obj;
    printf("Cat with claws of length %f was destroyed\n", c->claw_length);
    // base class destroy
    animal_destroy(obj);
}

void cat_voice(animal_t *obj) {
    cat_t *c = (cat_t *) obj;

    printf("%s: Meow\n", obj->name_);
}

void cat_scratch(cat_t *c) {
    printf("%s: Scratch with depth %f\n", c->super.name_, c->claw_length);
}

