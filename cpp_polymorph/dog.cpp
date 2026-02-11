#include "dog.h"
#include <iostream>

void dog::voice() {
    std::cout << name_ << ": Bark\n";
}

dog::dog() {
    set_name(DOG_DEFAULT_NAME);
    jump_height = rand();
}

dog::dog(const char *name) {
    set_name(name);
    jump_height = rand();
}

void dog::jump() {
    std::cout << name_ << ": Jumped on " << jump_height << "\n";
}
