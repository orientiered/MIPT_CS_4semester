#include <iostream>
#include <string>

#include "model.h"
#include "bots.h"

namespace sngm {

Snake& GameModel::spawnDefaultSnake(Coord offset, Direction dir) {
    std::list<Coord> body = {offset + Coord{2, 0}, offset + Coord{1, 0}, offset + Coord{0, 0}};
    snakes.push_back(Snake{body, dir});
    return snakes.back();
}

GameModel::GameModel(int32_t width_, int32_t height_, uint16_t spawn_controlled, uint16_t spawn_bot,
                     std::vector<BotType> bot_types)
{
    resize(width_, height_);
    controllable_snakes = std::min(spawn_controlled, MAX_CONTROLLABLE_SNAKES);

    for (int i = 0; i < controllable_snakes; i++) {
        spawnDefaultSnake({0, height * (i+1) / (controllable_snakes + 1)});
    }

    bot_snakes = std::min(spawn_bot, MAX_BOT_SNAKES);
    for (int i = 0; i < bot_snakes; i++) {
        BotType bot_type = (i < bot_types.size()) ? bot_types[i] : SNAKE_BOT_DUMB;

        switch (bot_type) {
            case SNAKE_BOT_DUMB:
                spawnSnakeBot<SnakeBot_Intuitive>();
                break;
            case SNAKE_BOT_MEDIUM:
                spawnSnakeBot<SnakeBot_Intuitive2>();
                break;
            default:
                std::cerr << "Invalid bot type";
                break;
        }
    }

}
//
CellInfo GameModel::checkCoord(Coord pos, bool update_cache) {
    //TODO:
    if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) {
        return {WallType, 0};
    }

    if (update_cache) cellsCache = buildOccupiedCells();

    auto cellIt = cellsCache.find(pos);
    if (cellIt == cellsCache.end()) return {EmptyType, 0};
    else return cellIt->second;
}

std::map<Coord, CellInfo> GameModel::buildOccupiedCells() {
    std::map<Coord, CellInfo> result;

    
    for (const Snake &snake: snakes) {
        if (!snake.isAlive) continue;
        auto it = snake.body.begin();
        result[*it] = {SnakeHeadType, snake.id_};
        it++;
        while (it != snake.body.end()) {
            result[*it] = {SnakeBodyType, snake.id_};
            it++;
        }
    }

    for (const Rabbit &rabbit: rabbits) {
        result[rabbit.pos] = {RabbitType, rabbit.id_};
    }

    return result;
}


void GameModel::spawnRabbits() {
    //TODO: may be optimized i guess
    // auto occupied_cells = buildOccupiedCells();

    int able_to_spawn = max_rabbit_count - rabbits.size();
    for (int i = 0; i < max_rabbit_spawn_tries && able_to_spawn > 0; i++) {
        int x = rng.range(0, width);
        int y = rng.range(0, height);
        if (checkCoord(Coord{x, y}).type == EmptyType) {
            rabbits.push_back(Rabbit({x, y}));
            cellsCache[{x,y}] = {RabbitType, rabbits.back().id_};
            able_to_spawn--;
        }
    }
}

void GameModel::kill_rabbit(Coord c) {
    for (auto rabbit_it = rabbits.begin(); rabbit_it != rabbits.end(); rabbit_it++) {
        if (rabbit_it->pos == c) {
            rabbits.erase(rabbit_it);
            break;
        }
    }
}

Snake* GameModel::getSnakeById(int64_t id) {
    for (Snake& snake: snakes) {
        if (snake.id_ == id) return &snake;
    }

    return nullptr;
}

Rabbit* GameModel::getRabbitById(int64_t id) {
       for (Rabbit& rab: rabbits) {
        if (rab.id_ == id) return &rab;
    }

    return nullptr;
}


void GameModel::tickStep() {
    if (!isValid()) return;


    if (snakes.size() == 0) {
        //TODO: game over + score
    }

    // updating cells cache
    checkCoord({0, 0}, true);

    spawnRabbits();

    // updating bots
    for (auto& snake_bot: bot_controllers) {
        snake_bot->tick(*this);
    }

    for (Snake& snake: snakes) {
        if (!snake.isAlive) continue;
        Coord nextCell = snake.getNextCell();
        CellInfo cell_info = checkCoord(nextCell);

        switch(cell_info.type) {
            case SnakeBodyType:
                snake.kill();
                if (cell_info.id != snake.id_)
                    score[cell_info.id] += scorePerKill;
                break;
            case SnakeHeadType:
            case WallType:
                snake.kill();
                break;
            case EmptyType:
                snake.step();
                break;
            case RabbitType:
                snake.grow();
                score[snake.id_] += scorePerRabbit;
                kill_rabbit(nextCell);
                break;
            default:
                break;
        }
    }

}

bool GameModel::isValid() const {
    return isValidSize;
}

const std::string_view GameModel::getErrorString() const {
    return error_string;
}

void GameModel::resize(int32_t new_width, int32_t new_height) {
    if (new_width < MIN_WIDTH || new_height < MIN_HEIGHT) {
        isValidSize = false;
        error_string = "Minimal size is " + std::to_string(MIN_WIDTH) + " x " + std::to_string(MIN_HEIGHT);
    } else {
        isValidSize = true;
        width = new_width;
        height = new_height;
    }
}

}
