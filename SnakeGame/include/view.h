#pragma once

#include "game_event.h"
#include "model.h"

namespace sngm {



class IView {
protected:
    IView() = default;
public:
    virtual void render(const GameModel& model) = 0;
    virtual GameEvent pollEvent() = 0;
};


}
