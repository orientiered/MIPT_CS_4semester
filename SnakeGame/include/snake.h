#pragma once

#include <cstddef>
#include <cstdint>
#include <list>

#include <coords.h>
#include <string>
namespace sngm {

class Snake {
public:
    std::list<Coord> body;
    Direction direction;
    std::string name = "snake";

    bool isAlive = true;

    // TODO: check opposite direction based on previous segment
    void setDirection(Direction new_dir) {
        if (isAlive && !isOppositeDirection(direction, new_dir))
            direction = new_dir;
    }

    Coord getNextCell() {
        return body.front() + direction;
    }

    void kill() {
        isAlive = false;
    }

    void step();
    void grow();
};

}
