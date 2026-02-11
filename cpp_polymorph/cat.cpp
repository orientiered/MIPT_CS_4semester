#include <assert.h>

#include "cat.h"
#include <iostream>

void cat::voice() {
    std::cout << name_ << ": Meow\n";
}

cat::cat() {
    set_name(CAT_DEFAULT_NAME);
    claw_length = rand();
}

cat::cat(const char *name) {
    set_name(name);

    claw_length = rand();
}

void cat::scratch() {
    std::cout << name_ << ": Scratch with depth " << claw_length << "\n";
}



