#pragma once

#include <variant>
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

struct WinchEvent {
    int width;
    int height;
};

struct GameEvent {
    std::variant<KeyEvent, WinchEvent> data_;

    template<typename T>
    bool is() const { return std::get_if<T>(&data_) != nullptr; }

    template<typename T>
    T& get() { return std::get<T>(data_); }

    template<typename T>
    const T& get() const { return std::get<T>(data_); }

};

}


