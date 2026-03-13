#pragma once

#include "game_event.h"
#include "model.h"
#include "view.h"
#include <chrono>

namespace sngm {

class GameController {
private:
    GameModel &model;
    IView &view;

    std::chrono::steady_clock clock;
    bool is_paused = false;
    bool exit_request = false;
    // std::chrono::duration<double> ;
public:
    GameController(GameModel& _model, IView& _view):
        model(_model), view(_view) {}

    void run();

private:
    void processEvent(const GameEvent& event);

};


}

