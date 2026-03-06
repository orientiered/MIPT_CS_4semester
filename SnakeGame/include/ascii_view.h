#pragma once
#include "view.h"
#include <memory>

namespace sngm {

class AsciiView final: public IView {
public:
    AsciiView();
    ~AsciiView();
    void render(const GameModel& model) override;
    std::optional<GameEvent> pollEvent() override;
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};


}
