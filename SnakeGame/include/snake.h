#pragma once

#include <cstddef>
#include <cstdint>
#include <list>

namespace sngm {

enum class Direction {UP, RIGHT, DOWN, LEFT};

class Snake {
public:
    std::list<std::pair<int32_t, int32_t>> body;
    size_t head_index;

    Direction direction;

    void step();
};

}
