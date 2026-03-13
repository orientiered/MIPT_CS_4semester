#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>

#include "snake.h"
#include "rabbit.h"

namespace sngm {

class GameModel {
public:
    GameModel(int32_t width_, int32_t height_): width(width_), height(height_) {}

    int32_t width, height;
    std::deque<Snake> snakes;
    std::deque<Rabbit> rabbits;

    void setP1SnakeDir(Direction dir) {
        snakes[0].direction = dir;
    }

    const Snake& getP1Snake() {
        return snakes[0];
    }

    void tickStep();



};


};
