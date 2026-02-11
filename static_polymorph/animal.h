#pragma once

typedef struct animal animal_t;

struct animal {
    char *name_;

    void (*voice)(animal_t *a);
    void (*destroy)(animal_t *a);
};

void animal_set_name(animal_t *a, const char *name);
void animal_destroy(animal_t *a);
