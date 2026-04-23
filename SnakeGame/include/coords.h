#pragma once
#include <cstdint>

enum class Direction {NONE, UP, RIGHT, DOWN, LEFT};
inline bool isOppositeDirection(Direction a, Direction b) {
    return a == Direction::UP && b == Direction::DOWN ||
           a == Direction::DOWN && b == Direction::UP ||
           a == Direction::LEFT && b == Direction::RIGHT ||
           a == Direction::RIGHT && b == Direction::LEFT;
}

inline Direction directionRotate90(Direction d) {
    return d == Direction::UP    ? Direction::RIGHT
        :  d == Direction::RIGHT ? Direction::DOWN
        :  d == Direction::DOWN  ? Direction::LEFT
        :  d == Direction::LEFT  ? Direction::UP :
                Direction::NONE;
}

inline float directionToDegree(Direction d) {
    switch (d) {
        case Direction::UP:
            return 0;
        case Direction::RIGHT:
            return 90;
        case Direction::DOWN:
            return 180;
        case Direction::LEFT:
            return 270;
        case Direction::NONE:
        default:
            return 0;
    }
}

struct Coord {
    int32_t x = 0;
    int32_t y = 0;

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

inline Coord& operator-=(Coord& c, const Coord other) {
    c.x -= other.x;
    c.y -= other.y;

    return c;
}

inline Coord operator-(const Coord& a, const Coord& b) {
    Coord result = a;
    result -= b;
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
