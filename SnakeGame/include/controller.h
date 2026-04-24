#pragma once

#include "game_event.h"
#include "model.h"
#include "view.h"
#include <chrono>

namespace sngm {

using namespace std::chrono_literals;

class GameController {
private:
    GameModel &model;
    IView &view;

    std::chrono::steady_clock clock;

    using time_point_t = typeof(clock.now());

    std::chrono::milliseconds tickPeriod = 200ms;
    std::chrono::milliseconds renderPeriod = 33ms;

    time_point_t next_tick;
    time_point_t next_render;

    bool is_paused = false;
    bool exit_request = false;

    bool tournament_mode = false;

public:
    GameController(GameModel& _model, IView& _view, bool tournament):
        model(_model), view(_view), tournament_mode(tournament) {
            if (tournament_mode) {
                tickPeriod = 5ms;
            }

            next_tick = clock.now();
            next_render = clock.now();

        }

    void run();

private:
    void processEvent(const GameEvent& event);

};


}

