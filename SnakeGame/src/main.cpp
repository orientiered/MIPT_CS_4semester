#include <iostream>

#include "ascii_view.h"
#include "controller.h"
#include "snake.h"

int main() {
    sngm::GameModel model(40, 30);
    sngm::Snake snake{std::list<sngm::Coord>{{11, 10}, {11, 11}, {11, 12}} , sngm::Direction::DOWN};
    model.snakes.push_front(snake);
    sngm::AsciiView view;

    sngm::GameController controller(model, view);

    controller.run();

    return 0;
}
