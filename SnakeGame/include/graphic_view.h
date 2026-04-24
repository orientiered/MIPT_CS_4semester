#pragma once
#include "view.h"
#include <filesystem>
#include <memory>

namespace sngm {

static const std::filesystem::path FONT_PATH  = std::filesystem::path("data") / "Play-Regular.ttf";
static const std::filesystem::path ATLAS_PATH = std::filesystem::path("data") / "atlas.png";

class GraphicView final: public IView {
public:
    GraphicView(uint32_t screen_width = 1920, uint32_t screen_height = 1080);
    ~GraphicView();

    virtual void render(const GameModel& model) override;
    virtual std::optional<GameEvent> pollEvent() override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};


}
