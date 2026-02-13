#pragma once

#include "animal.h"

struct cat {
    animal_t super;
    float claw_length;
};

typedef struct cat cat_t;

static const char * const CAT_DEFAULT_NAME = "Pickle";

void cat_init(cat_t *c);
void cat_init_named(cat_t *c, const char *name);

void cat_destroy(animal_t *obj);
void cat_voice(animal_t *obj);
void cat_scratch(cat_t *c);
