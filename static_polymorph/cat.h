#pragma once

#include "animal.h"

struct cat {
    animal_t super;
    float claw_length;
};

typedef struct cat cat_t;

static const char * const CAT_DEFAULT_NAME = "Pickle";

cat_t *cat_create();
cat_t *cat_create_named(const char *name);
void cat_destroy(animal_t *obj);
void cat_voice(animal_t *obj);
void cat_scratch(cat_t *c);
