#pragma once

struct cat {
    char *name_;
};

typedef struct cat cat_t;

const char * const CAT_DEFAULT_NAME = "Pickle";

cat_t cat_create();
cat_t cat_create_named(const char *name);
void cat_destroy(cat_t *obj);
void cat_voice(cat_t *obj);
