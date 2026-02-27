#pragma once

namespace sngm {

enum class KeyEvent {
    P1_UP,
    P1_RIGHT,
    P1_DOWN,
    P1_LEFT,

    P2_UP,
    P2_RIGHT,
    P2_DOWN,
    P2_LEFT,

    RESTART,
    PAUSE,
    EXIT
};

struct GameEvent {
    KeyEvent key;
};

}
