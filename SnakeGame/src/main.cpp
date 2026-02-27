#include <iostream>

#include "ascii_view.h"
#include "controller.h"

int main() {
    sngm::GameModel model;
    sngm::AsciiView view;

    sngm::GameController controller(model, view);

    controller.run();

    return 0;
}
