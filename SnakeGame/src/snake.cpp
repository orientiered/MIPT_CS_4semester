#include "snake.h"

namespace sngm {

void Snake::step() {
    if (!isAlive) return;
    body.pop_back();
    body.push_front(body.front() + direction);
}


}
