#pragma once

#include "game_event.h"
#include "model.h"
#include "view.h"

namespace sngm {

class GameController {
private:
    GameModel &model;
    IView &view;

public:
    GameController(GameModel& _model, IView& _view):
        model(_model), view(_view) {}

    void run();

private:
    void processEvent(const GameEvent& event);

};


}

