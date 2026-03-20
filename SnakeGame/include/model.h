#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>

#include "snake.h"
#include "rabbit.h"

namespace sngm {

class GameModel {
public:
    GameModel(int32_t width_, int32_t height_, uint16_t spawn_controlled = 1, uint16_t spawn_bot = 0);

    int32_t width, height;
    std::deque<Snake> snakes;
    std::deque<Rabbit> rabbits;

    int controllable_snakes = 1;
    static inline const uint16_t MAX_CONTROLLABLE_SNAKES = 2;
    int bot_snakes = 0;
    static inline const uint16_t MAX_BOT_SNAKES = 10;

    void tickStep();
    void setPlayerSnakeDir(int snakeId, Direction dir) {
        if (snakeId < controllable_snakes)
            snakes[snakeId].direction = dir;
    }
private:
    void spawnDefaultSnake(Coord offset, Direction dir = Direction::RIGHT);

};


};
