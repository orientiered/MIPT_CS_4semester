#include "graphic_view.h"
#include <memory>

//TODO:
// #include <SFML/

namespace sngm {

struct GraphicView::Impl {
    int placeholder;
};

void GraphicView::render(const GameModel& model) {

}

GameEvent GraphicView::pollEvent() {
    return {KeyEvent::EXIT};
}

GraphicView::GraphicView(uint32_t screen_width, uint32_t screen_height): impl_(std::make_unique<Impl>()) {}
GraphicView::~GraphicView() = default;


}
