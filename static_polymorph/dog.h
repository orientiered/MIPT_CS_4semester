#pragma once

#include "animal.h"

struct dog {
    animal_t super;
    float jump_height;
};

typedef struct dog dog_t;

static const char * const DOG_DEFAULT_NAME = "Boris";
dog_t *dog_create();
dog_t *dog_create_named(const char *name);
void dog_destroy(animal_t *obj);
void dog_voice(animal_t *obj);
void dog_jump(dog_t *d);
