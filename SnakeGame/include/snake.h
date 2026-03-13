#pragma once

#include <cstddef>
#include <cstdint>
#include <list>

namespace sngm {

enum class Direction {UP, RIGHT, DOWN, LEFT};

struct Coord {
    int32_t x;
    int32_t y;
};

inline Coord& operator+=(Coord& c, const Direction& dir) {
    switch (dir) {
        case Direction::UP:
            c.y++;
            break;
        case Direction::DOWN:
            c.y--;
            break;
        case Direction::RIGHT:
            c.x++;
            break;
        case Direction::LEFT:
            c.x--;
            break;
    }
    return  c;
}

inline Coord operator+(const Coord& c, const Direction& dir) {
    Coord new_coord = c;
    new_coord += dir;
    return new_coord;
}

class Snake {
public:
    std::list<Coord> body;

    Direction direction;

    void step();
};

}
