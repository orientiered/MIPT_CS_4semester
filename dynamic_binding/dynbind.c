#include "cat.h"
#include "dog.h"
#include "obj.h"

int main() {
      class_t *obj_cls = class_create(NULL, "obj", sizeof(obj_t));

      class_t *animal_cls = class_create(obj_cls, "animal", sizeof(animal_t));

      CLASS_ADD_DTOR(animal_cls, animal_destroy);
      CLASS_ADD_METHOD(animal_cls, "set_name", animal_set_name);

      class_t *dog_cls = class_create(animal_cls, "dog", sizeof(dog_t));

      CLASS_ADD_METHOD(dog_cls, "voice", dog_voice);
      CLASS_ADD_METHOD(dog_cls, "jump", dog_jump);
      CLASS_ADD_INITOR(dog_cls, dog_init);

      class_t *cat_cls = class_create(animal_cls, "cat", sizeof(cat_t));

      CLASS_ADD_METHOD(cat_cls, "voice", cat_voice);
      CLASS_ADD_METHOD(cat_cls, "scratch", cat_scratch);
      CLASS_ADD_DTOR(cat_cls, cat_destroy);
      CLASS_ADD_INITOR(cat_cls, cat_init);

      obj_t* zoo[] = {
            obj_create(cat_cls),
            obj_create(cat_cls),
            obj_create(dog_cls),
            obj_create(dog_cls),
            // (animal_t*) cat_create(),
            // (animal_t*) cat_create_named("Berserk"),
            // (animal_t*) dog_create(),
            // (animal_t*) dog_create_named("Dog 2")
      };

      obj_invoke_1ptr(zoo[1], "set_name", (void *) "Berserk");
      obj_invoke_1ptr(zoo[3], "set_name", (void *) "Dog 2");

      for (int i = 0; i < 4; i++) {
            obj_invoke_0ptr(zoo[i], "voice");

      }

      obj_invoke_0ptr(zoo[1], "scratch");
      obj_invoke_0ptr(zoo[3], "jump");

      for (int i = 0; i < 4; i++) {
            obj_destroy(zoo[i]);
      }

    return 0;
}
