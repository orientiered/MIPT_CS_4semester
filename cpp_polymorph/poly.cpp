#include "cat.h"
#include "dog.h"
#include <memory>

int main() {

      std::unique_ptr<animal> zoo[] = {
            std::make_unique<cat>(),
            std::make_unique<cat>("Berserk"),
            std::make_unique<dog>(),
            std::make_unique<dog>("Dog 2"),
      };

      for (int i = 0; i < 4; i++) {
            zoo[i]->voice();
      }

      dynamic_cast<cat&>(*zoo[1]).scratch();
      dynamic_cast<dog&>(*zoo[3]).jump();

    return 0;
}
