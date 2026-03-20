#include "model.h"

namespace sngm {

void GameModel::spawnDefaultSnake(Coord offset, Direction dir) {
    std::list<Coord> body = {offset + Coord{2, 0}, offset + Coord{1, 0}, offset + Coord{0, 0}};
    snakes.push_back(Snake{body, dir});
}

GameModel::GameModel(int32_t width_, int32_t height_, uint16_t spawn_controlled, uint16_t spawn_bot):
    width(width_), height(height_) {
    controllable_snakes = std::min(spawn_controlled, MAX_CONTROLLABLE_SNAKES);

    for (int i = 0; i < controllable_snakes; i++) {
        spawnDefaultSnake({height * (i+1) / (controllable_snakes + 1), 0});
    }

    bot_snakes = std::min(spawn_bot, MAX_BOT_SNAKES);
}

void GameModel::tickStep() {
    for (Snake& snake: snakes) {
        snake.step();
    }
}

}
