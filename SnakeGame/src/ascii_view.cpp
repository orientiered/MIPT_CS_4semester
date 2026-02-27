#include "ascii_view.h"
#include <memory>

namespace sngm {

struct AsciiView::Impl {
    int placeholder;
};

void AsciiView::render(const GameModel& model) {

}

GameEvent AsciiView::pollEvent() {
    return {KeyEvent::EXIT};
}

AsciiView::AsciiView(): impl_(std::make_unique<Impl>()) {}
AsciiView::~AsciiView() = default;


}
