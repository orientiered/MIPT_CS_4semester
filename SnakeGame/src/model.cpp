#include "model.h"

namespace sngm {

void GameModel::tickStep() {
    for (Snake& snake: snakes) {
        snake.step();
    }
}

}
