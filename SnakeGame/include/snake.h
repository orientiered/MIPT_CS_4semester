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

    void step();
};

}
