#pragma once

struct dog {
    char *name_;
};


typedef struct dog dog_t;

const char * const DOG_DEFAULT_NAME = "Boris";
dog_t dog_create();
dog_t dog_create_named(const char *name);
void dog_destroy(dog_t *obj);
void dog_voice(dog_t *obj);
