#include "snake.h"

namespace sngm {

void Snake::step() {
    body.pop_back();
    body.push_front(body.front() + direction);
}


}
