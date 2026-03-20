#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "snake.h"
#include "rabbit.h"

#include "simple_random.h"

namespace sngm {

enum CellType {SnakeBodyType, SnakeHeadType, RabbitType, WallType, EmptyType};


class GameModel {
public:
    GameModel(int32_t width_, int32_t height_, uint16_t spawn_controlled = 1, uint16_t spawn_bot = 0);

    int32_t width, height;
    std::deque<Snake> snakes;
    std::deque<Rabbit> rabbits;

    static inline const int MIN_WIDTH = 40;
    static inline const int MIN_HEIGHT = 20;

    int controllable_snakes = 1;
    static inline const uint16_t MAX_CONTROLLABLE_SNAKES = 2;
    int bot_snakes = 0;
    static inline const uint16_t MAX_BOT_SNAKES = 10;

    uint16_t max_rabbit_count = 3;
    uint16_t max_rabbit_spawn_tries = 1;

    FastRng rng;

    void tickStep();

    void setPlayerSnakeDir(int snakeId, Direction dir) {
        if (snakeId < controllable_snakes)
            snakes[snakeId].setDirection(dir);
    }


    /// @brief Get error description if model is not valid
    const std::string_view getErrorString() const;

    bool isValid() const;

    // Try to resize model to new_width x new_height
    void resize(int32_t new_width, int32_t new_height);
private:
    bool isValidSize = true;
    std::string error_string;

    void spawnRabbits();

    std::map<Coord, CellType> buildOccupiedCells();

    // CellObj checkCoord(Coord pos);

    void spawnDefaultSnake(Coord offset, Direction dir = Direction::RIGHT);

};


};
