#pragma once

#include <cstddef>
#include <cstdint>
#include <list>

#include <coords.h>
#include <string>
namespace sngm {

using SnakeId = int64_t;

class Snake {
private:
    static SnakeId unique_id_;
public:
    SnakeId id_;

    Snake(std::list<Coord> b, Direction dir): id_(unique_id_++), body(b), direction(dir) {}

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

inline SnakeId Snake::unique_id_ = 0;

}
