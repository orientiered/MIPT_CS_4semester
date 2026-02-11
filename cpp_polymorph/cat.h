#pragma once

#include "animal.h"

static const char * const CAT_DEFAULT_NAME = "Pickle";

struct cat : animal {
    float claw_length;
    cat();
    cat(const char *name);
    void voice() override;
    void scratch();

    ~cat() override = default;
};


