#pragma once
#include <cstdint>

enum class Direction {UP, RIGHT, DOWN, LEFT};

struct Coord {
    int32_t x;
    int32_t y;

    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Coord& other) const {
        if (x < other.x) return true;
        if (x == other.x) return y < other.y;
        return false;
    }
};

inline Coord& operator+=(Coord& c, const Coord other) {
    c.x += other.x;
    c.y += other.y;

    return c;
}

inline Coord operator+(const Coord& a, const Coord& b) {
    Coord result = a;
    result += b;
    return result;
}


inline Coord& operator+=(Coord& c, const Direction& dir) {
    switch (dir) {
        case Direction::UP:
            c.y++;
            break;
        case Direction::DOWN:
            c.y--;
            break;
        case Direction::RIGHT:
            c.x++;
            break;
        case Direction::LEFT:
            c.x--;
            break;
    }
    return  c;
}

inline Coord operator+(const Coord& c, const Direction& dir) {
    Coord new_coord = c;
    new_coord += dir;
    return new_coord;
}
