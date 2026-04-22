#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

#include "coords.h"

namespace sngm {


class Rabbit {
private:
    static int64_t unique_id_;
public:
    int64_t id_;
    Coord pos;

    Rabbit(Coord c): id_(unique_id_++), pos(c) {}
};

inline int64_t Rabbit::unique_id_ = 0;

}
