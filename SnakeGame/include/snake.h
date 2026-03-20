#pragma once

#include <cstddef>
#include <cstdint>
#include <list>

#include <coords.h>
namespace sngm {

class Snake {
public:
    std::list<Coord> body;
    Direction direction;

    bool isAlive = true;


    void setDirection(Direction new_dir) {
        if (isAlive && !isOppositeDireciton(direction, new_dir))
            direction = new_dir;
    }
    void step();
};

}
