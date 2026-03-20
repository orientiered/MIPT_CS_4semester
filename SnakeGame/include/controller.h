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

    static constexpr auto tickPeriod = 200ms;
    static constexpr auto renderPeriod = 33ms;

    time_point_t next_tick;
    time_point_t next_render;

    bool is_paused = false;
    bool exit_request = false;
    // std::chrono::duration<double> ;
public:
    GameController(GameModel& _model, IView& _view):
        model(_model), view(_view) {
            next_tick = clock.now();
            next_render = clock.now();
        }

    void run();

private:
    void processEvent(const GameEvent& event);

};


}

