#include <cassert>
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
    bot_types_ = bot_types;

    resize(width_, height_);
    controllable_snakes = std::min(spawn_controlled, MAX_CONTROLLABLE_SNAKES);

    bot_snakes = std::min(spawn_bot, MAX_BOT_SNAKES);
    restart();
}

void GameModel::restart() {
    snakes.clear();
    rabbits.clear();
    bot_controllers.clear();
    score.clear();

    for (int i = 0; i < controllable_snakes; i++) {
        spawnDefaultSnake({0, height * (i+1) / (controllable_snakes + 1)});
    }

    for (int i = 0; i < bot_snakes; i++) {
        BotType bot_type = (i < bot_types_.size()) ? bot_types_[i] : SNAKE_BOT_DUMB;

        switch (bot_type) {
            case SNAKE_BOT_DUMB:
                spawnSnakeBot<SnakeBot_Intuitive>();
                break;
            case SNAKE_BOT_MEDIUM:
                spawnSnakeBot<SnakeBot_Intuitive2>();
                break;
            case SNAKE_BOT_BFS:
                spawnSnakeBot<SnakeBot_BFS>();
                break;
            default:
                std::cerr << "Invalid bot type";
                break;
        }
    }

    updateCache();
}

//

void GameModel::updateCache() {
    cellsCache = buildOccupiedCells();
}

CellInfo GameModel::checkCoord(Coord pos) const {
    //TODO:
    if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) {
        return {WallType, 0};
    }

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

        result[snake.body.back()] = {SnakeTailType, snake.id_}; // overwriting tail
    }

    for (const Rabbit &rabbit: rabbits) {
        result[rabbit.pos] = {RabbitType, rabbit.id_};
    }

    if (lazer_turret) 
        result[lazer_turret->pos] = {LaserPickupType, 0};

    return result;
}

void GameModel::spawnLazer() {
    if (lazer_turret) return;

    if (rng.range(0, 1000) <= 10) {
        int x = rng.range(0, width);
        int y = rng.range(0, height);
        if (checkCoord(Coord{x, y}).type == EmptyType) {
            lazer_turret = LazerTurret({x, y});
            cellsCache[{x,y}] = {LaserPickupType, 0};
        }
    }
}


void GameModel::handleLazerShoot(Direction dir) {
    if (!lazer_turret) return;

    Coord pos = lazer_turret->pos;
    while (true) {
        pos += dir;
        CellInfo info = checkCoord(pos);

        if (info.type == WallType) break;
        switch (info.type) {
            case SnakeHeadType:
            case SnakeBodyType:
            case SnakeTailType:
                killSnakeCached(*getSnakeById(info.id));
                break;
            default:
                break;
        }

    }

    lazer_turret = std::nullopt;
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


void GameModel::killSnakeCached(Snake &s) {
    s.kill();
    for (Coord c: s.body) {
        cellsCache.erase(c); 
    }
}

void GameModel::stepSnakeCached(Snake &s) {
    cellsCache.erase(s.body.back());
    s.step();
    cellsCache[s.body.back()] = {SnakeTailType, s.id_};
    cellsCache[s.body.front()] = {SnakeHeadType, s.id_};
}

void GameModel::growSnakeCached(Snake &s) {
    cellsCache[s.body.front()] = {SnakeBodyType, s.id_};
    s.grow();
    cellsCache[s.body.front()] = {SnakeHeadType, s.id_};
}

void GameModel::tickStep() {
    if (!isValid()) return;


    if (snakes.size() == 0) {
        //TODO: game over + score
    }

    // updating cells cache
    // updateCache();

    spawnLazer();
    spawnRabbits();

    // updating bots
    for (auto& snake_bot: bot_controllers) {
        snake_bot->tick(*this);
    }

    std::map<Coord, std::vector<SnakeId>> planned_heads;
    
    // Phase 1: calculating new head positions
    for (Snake& snake: snakes) {
        if (!snake.isAlive) continue;
        snake.will_grow = false;

        Coord next_cell = snake.getNextCell();
        switch (checkCoord(next_cell).type) {
            case WallType:
                killSnakeCached(snake);
                break;
            case RabbitType:
                snake.will_grow = true; // wants to grow
            default:
                planned_heads[next_cell].push_back(snake.id_);
                break;
        }
    }

    // Phase 2: Killing all snakes that will collide WITH HEADS
    for (auto& [coord, ids]: planned_heads) {
        if (ids.size() > 1) {
            for (SnakeId id : ids) {
                Snake * s= getSnakeById(id);
                killSnakeCached(*s);
            }

        }
    }


    // Phase 3: resolving other conflicts and moving snakes
    for (Snake& snake: snakes) {
        if (!snake.isAlive) continue;
        Coord nextCell = snake.getNextCell();
        CellInfo next_cell_info = checkCoord(nextCell);

        SnakeId other_id = next_cell_info.id;
        bool other_is_not_me = other_id != snake.id_;

        // Processing next cell
        switch(next_cell_info.type) {
            case SnakeTailType: 
            {
                Snake *other = getSnakeById(other_id);
                if (other->will_grow) {
                    killSnakeCached(snake);
                    score[other_id] += scorePerKill * other_is_not_me;
                } else {
                    stepSnakeCached(snake);
                }
            }
            break;
            case SnakeBodyType:
            case SnakeHeadType:
                killSnakeCached(snake);
                score[other_id] += scorePerKill * other_is_not_me;
                break;
            case WallType:
                killSnakeCached(snake);
                break;
            case EmptyType:
                stepSnakeCached(snake);
                break;
            case LaserPickupType:
                stepSnakeCached(snake);
                handleLazerShoot(snake.direction);
                break;
            case RabbitType:
                growSnakeCached(snake);
                score[snake.id_] += scorePerRabbit;
                kill_rabbit(nextCell); // rabbit cache is overwritten by growing snake
                break;
            default:
                break;
        }
    }

}

size_t GameModel::aliveSnakes() {
    size_t result = 0;
    for (Snake& snake : snakes) {
        result += snake.isAlive;
    }

    return result;
}

std::vector<std::pair<Score_t, BotType>> GameModel::exportScores() const {
    // TODO: wont work for game with players
    std::vector<std::pair<Score_t, BotType>> result(snakes.size());
    for (int i = 0; i < snakes.size(); i++) {

        result[i] = {getScore(snakes[i].id_), bot_types_[i]};
    }

    return result;
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
