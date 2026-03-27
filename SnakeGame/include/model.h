#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "snake.h"
#include "rabbit.h"

#include "simple_random.h"

namespace sngm {

//EmptyType must be zero for default construction
enum CellType {EmptyType = 0, SnakeBodyType, SnakeHeadType, RabbitType, WallType};

class GameModel;

class ISnakeBot {
protected:
    ISnakeBot(Snake& snake): controlled(&snake) {}
public:
    Snake *controlled;
    virtual void tick(GameModel &model) = 0;

    virtual ~ISnakeBot() = default;
};

class GameModel {
public:
    GameModel(int32_t width_, int32_t height_, uint16_t spawn_controlled = 1, uint16_t spawn_bot = 0);

    int32_t width, height;

    const std::deque<Snake>& getSnakes() const {
        return snakes;
    }
    const std::deque<Rabbit>& getRabbits() const {
        return rabbits;
    }

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
    std::deque<Snake> snakes;
    std::deque<Rabbit> rabbits;

    std::vector<std::unique_ptr<ISnakeBot>> bot_controllers;

    bool isValidSize = true;
    std::string error_string;

    void spawnRabbits();

    std::map<Coord, CellType> buildOccupiedCells();

    void kill_rabbit(Coord c);
    // CellObj checkCoord(Coord pos);

    Snake& spawnDefaultSnake(Coord offset, Direction dir = Direction::RIGHT);

    template <typename SnakeBotT>
    Snake& spawnSnakeBot() {
        Snake& new_bot = spawnDefaultSnake(Coord{rng.range(0, width), rng.range(0, height)});
        bot_controllers.push_back(std::make_unique<SnakeBotT>(new_bot));
        return new_bot;
    }

};


};
