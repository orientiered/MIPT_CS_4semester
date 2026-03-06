#include "controller.h"
#include <chrono>
#include <iostream>
#include <thread>

namespace sngm {

void GameController::run() {
    using namespace std::chrono_literals;
    while (!exit_request) {

        while (std::optional<GameEvent> event = view.pollEvent()) {
            // std::cout << "processing event with code " << static_cast<int>(event->key) << "\n";
            processEvent(*event);
        }

        view.render(model);
        std::this_thread::sleep_for(16ms);
    }


}

void GameController::processEvent(const GameEvent& event) {
    switch (event.key) {
        case KeyEvent::EXIT:
            exit_request = true;
            break;

        default:
            break;
    }
}



}
