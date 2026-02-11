#include "cat.h"
#include "dog.h"

int main() {

      animal_t* zoo[] = {
            (animal_t*) cat_create(),
            (animal_t*) cat_create_named("Berserk"),
            (animal_t*) dog_create(),
            (animal_t*) dog_create_named("Dog 2")
      };

      for (int i = 0; i < 4; i++) {
            zoo[i]->voice(zoo[i]);

      }

      cat_scratch((cat_t *)zoo[1]);
      dog_jump((dog_t *)zoo[3]);

      for (int i = 0; i < 4; i++) {
            zoo[i]->destroy(zoo[i]);

      }

    return 0;
}
