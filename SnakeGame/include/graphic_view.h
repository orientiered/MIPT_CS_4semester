#pragma once
#include "view.h"
#include <memory>

namespace sngm {

class GraphicView final: public IView {
public:
    GraphicView(uint32_t screen_width = 1280, uint32_t screen_height = 720);
    ~GraphicView();

    virtual void render(const GameModel& model) override;
    virtual GameEvent pollEvent() override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};


}
