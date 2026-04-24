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
enum CellType {EmptyType = 0, SnakeBodyType, SnakeHeadType, SnakeTailType, RabbitType, WallType};

struct CellInfo {
    CellType type;
    int64_t id;
};

class GameModel;

/* =============== Bot related ======================= */
class ISnakeBot {
protected:
    ISnakeBot(Snake& snake): controlled(&snake) {}
public:
    Snake *controlled;
    virtual void tick(GameModel &model) = 0;

    virtual ~ISnakeBot() = default;
};

enum BotType {
    SNAKE_BOT_DUMB = 0,
    SNAKE_BOT_MEDIUM,
    SNAKE_BOT_BFS
};

const BotType MIN_BOT_TYPE = SNAKE_BOT_DUMB;
const BotType MAX_BOT_TYPE = SNAKE_BOT_BFS;

static inline std::string botTypeToString(BotType bot) {
    switch (bot) {
        case SNAKE_BOT_DUMB:   return "DUMB";
        case SNAKE_BOT_MEDIUM: return "MEDIUM";
        case SNAKE_BOT_BFS:    return "BFSBOT";
        default:               return "UNKNOWN";
    }
}

using Score_t = int;

using RunStats = std::vector<std::pair<Score_t, BotType>>;

class GameModel {
public:
    GameModel(int32_t width_, int32_t height_, uint16_t spawn_controlled = 1, uint16_t spawn_bot = 0,
              std::vector<BotType> bot_types = std::vector<BotType>());

    int32_t width, height;

    Snake* getSnakeById(SnakeId id);
    Rabbit* getRabbitById(int64_t id);
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
    void restart();

    void setPlayerSnakeDir(int snakeId, Direction dir) {
        if (snakeId < controllable_snakes)
            snakes[snakeId].setDirection(dir);
    }


    /// @brief Get error description if model is not valid
    const std::string_view getErrorString() const;

    bool isValid() const;
    size_t aliveSnakes();

    // Try to resize model to new_width x new_height
    void resize(int32_t new_width, int32_t new_height);
    CellInfo checkCoord(Coord pos) const;

    Score_t getScore(SnakeId id) const { 
        auto it = score.find(id);
        if (it != score.end()) return it->second;

        return {};
    }  

    RunStats exportScores() const;

    const Score_t scorePerRabbit = 10;
    const Score_t scorePerKill = 30;
private:
    std::map<SnakeId, Score_t> score;
    std::deque<Snake> snakes;
    std::deque<Rabbit> rabbits;

    std::vector<std::unique_ptr<ISnakeBot>> bot_controllers;
    std::vector<BotType> bot_types_;

    bool isValidSize = true;
    std::string error_string;

    void spawnRabbits();

    std::map<Coord, CellInfo> buildOccupiedCells();
    std::map<Coord, CellInfo> cellsCache;
    void updateCache();

    void kill_rabbit(Coord c);
    /// update cache after action
    void killSnakeCached(Snake &s);
    void stepSnakeCached(Snake &s);
    void growSnakeCached(Snake &s);

    Snake& spawnDefaultSnake(Coord offset, Direction dir = Direction::RIGHT);

    template <typename SnakeBotT>
    Snake& spawnSnakeBot() {
        Snake& new_bot = spawnDefaultSnake(Coord{rng.range(0, width), rng.range(0, height)});
        bot_controllers.push_back(std::make_unique<SnakeBotT>(new_bot));
        return new_bot;
    }

};


};
