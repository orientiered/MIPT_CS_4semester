#pragma once

#include "animal.h"

static const char * const DOG_DEFAULT_NAME = "Boris";

struct dog : animal {
    float jump_height;
    dog();
    dog(const char *name);
    void voice() override;
    void jump();

    ~dog() override = default;
};
