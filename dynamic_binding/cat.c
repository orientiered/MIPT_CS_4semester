#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "cat.h"


void cat_init(cat_t *c) {
    cat_init_named(c, CAT_DEFAULT_NAME);
}

void cat_init_named(cat_t *c, const char *name) {
    animal_set_name(&c->super, name);
    c->claw_length = rand();

}

void cat_destroy(animal_t *obj) {
    // child class destroy
    cat_t *c = (cat_t *) obj;
    printf("Cat with claws of length %f was destroyed\n", c->claw_length);
}

void cat_voice(animal_t *obj) {
    cat_t *c = (cat_t *) obj;

    printf("%s: Meow\n", obj->name_);
}

void cat_scratch(cat_t *c) {
    printf("%s: Scratch with depth %f\n", c->super.name_, c->claw_length);
}

