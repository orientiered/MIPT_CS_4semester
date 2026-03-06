#pragma once

#include "game_event.h"
#include "model.h"
#include <optional>

namespace sngm {



class IView {
protected:
    IView() = default;
public:
    virtual void render(const GameModel& model) = 0;
    virtual std::optional<GameEvent> pollEvent() = 0;
};


}
