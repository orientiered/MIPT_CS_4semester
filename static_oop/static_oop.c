#include "cat.h"
#include "dog.h"

int main() {

    cat_t c1 = cat_create(),
          c2 = cat_create_named("Berserk");

    dog_t d1 = dog_create(),
          d2 = dog_create_named("Dog 2");


    cat_voice(&c1);
    cat_voice(&c2);
    dog_voice(&d1);
    dog_voice(&d2);

    cat_destroy(&c1);
    cat_destroy(&c2);

    dog_destroy(&d1);
    dog_destroy(&d2);

    return 0;
}
