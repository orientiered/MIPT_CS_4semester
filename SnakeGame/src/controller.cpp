#include "controller.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <variant>

namespace sngm {

void GameController::run() {
    using namespace std::chrono_literals;


    while (!exit_request) {
        auto current_time = clock.now();

        while (std::optional<GameEvent> event = view.pollEvent()) {
            // std::cout << "processing event with code " << static_cast<int>(event->key) << "\n";
            processEvent(*event);
        }

        if (next_tick <= current_time && !is_paused) {
            model.tickStep();
            next_tick = current_time + tickPeriod;
        }

        if (next_render <= current_time) {
            view.render(model);
            next_render = current_time + renderPeriod;
        }

        std::this_thread::sleep_until(std::min(next_render, next_tick));
    }


}

void GameController::processEvent(const GameEvent& event) {
    if (event.is<KeyEvent>()) {
        KeyEvent key = event.get<KeyEvent>();
        switch (key) {
            case KeyEvent::EXIT:
                exit_request = true;
                break;
            case KeyEvent::P1_UP:
                model.setPlayerSnakeDir(0, Direction::UP);
                break;
            case KeyEvent::P1_DOWN:
                model.setPlayerSnakeDir(0, Direction::DOWN);
                break;
            case KeyEvent::P1_LEFT:
                model.setPlayerSnakeDir(0, Direction::LEFT);
                break;
            case KeyEvent::P1_RIGHT:
                model.setPlayerSnakeDir(0, Direction::RIGHT);
                break;
            case KeyEvent::P2_UP:
                model.setPlayerSnakeDir(1, Direction::UP);
                break;
            case KeyEvent::P2_DOWN:
                model.setPlayerSnakeDir(1, Direction::DOWN);
                break;
            case KeyEvent::P2_LEFT:
                model.setPlayerSnakeDir(1, Direction::LEFT);
                break;
            case KeyEvent::P2_RIGHT:
                model.setPlayerSnakeDir(1, Direction::RIGHT);
                break;
            case KeyEvent::PAUSE:
                is_paused = !is_paused;
                break;
            default:
                break;
        }
    } else if (event.is<WinchEvent>()) {
        WinchEvent ws = event.get<WinchEvent>();
        model.resize(ws.width, ws.height);
    }
}



}
