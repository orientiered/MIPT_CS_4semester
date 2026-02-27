#pragma once

#include <cstddef>
#include <cstdint>
#include <list>

#include "snake.h"
#include "rabbit.h"

namespace sngm {

class GameModel {

    int32_t width, height;
    std::list<Snake> snakes;
    std::list<Rabbit> rabbits;

    void tickStep();



};


};
