#include <iostream>

#include "ascii_view.h"
#include "controller.h"
#include "snake.h"

int main(int argc, const char *argv[]) {

    sngm::GameModel model(40, 30, 0, 5);

    sngm::AsciiView view;

    sngm::GameController controller(model, view);

    controller.run();

    return 0;
}
