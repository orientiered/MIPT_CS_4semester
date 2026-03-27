#include "model.h"
#include <string>

namespace sngm {

void GameModel::spawnDefaultSnake(Coord offset, Direction dir) {
    std::list<Coord> body = {offset + Coord{2, 0}, offset + Coord{1, 0}, offset + Coord{0, 0}};
    snakes.push_back(Snake{body, dir});
}

GameModel::GameModel(int32_t width_, int32_t height_, uint16_t spawn_controlled, uint16_t spawn_bot)
{
    resize(width_, height_);
    controllable_snakes = std::min(spawn_controlled, MAX_CONTROLLABLE_SNAKES);

    for (int i = 0; i < controllable_snakes; i++) {
        spawnDefaultSnake({0, height * (i+1) / (controllable_snakes + 1)});
    }

    bot_snakes = std::min(spawn_bot, MAX_BOT_SNAKES);
}
//
// CellObj GameModel::checkCoord(Coord pos) {
//     //TODO:
//     return EmptyObj;
// }

std::map<Coord, CellType> GameModel::buildOccupiedCells() {
    std::map<Coord, CellType> result;

    for (const Snake &snake: snakes) {
        auto it = snake.body.begin();
        result[*it] = SnakeHeadType;
        it++;
        while (it != snake.body.end()) {
            result[*it] = SnakeBodyType;
            it++;
        }
    }

    for (const Rabbit &rabbit: rabbits) {
        result[rabbit.pos] = RabbitType;
    }

    return result;
}


void GameModel::spawnRabbits() {
    //TODO: may be optimized i guess
    auto occupied_cells = buildOccupiedCells();

    int able_to_spawn = max_rabbit_count - rabbits.size();
    for (int i = 0; i < max_rabbit_spawn_tries && able_to_spawn > 0; i++) {
        int x = rng.range(0, width);
        int y = rng.range(0, height);
        if (occupied_cells.find(Coord{x, y}) == occupied_cells.end()) {
            rabbits.push_back(Rabbit{x, y});
            occupied_cells[{x,y}] = RabbitType;
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


void GameModel::tickStep() {
    if (!isValid()) return;


    if (snakes.size() == 0) {
        //TODO: game over + score
    }

    spawnRabbits();

    auto occupied_cells = buildOccupiedCells();

    for (Snake& snake: snakes) {
        if (!snake.isAlive) continue;
        Coord nextCell = snake.getNextCell();
        switch(occupied_cells[nextCell]) {
            case SnakeBodyType:
            case SnakeHeadType:
                snake.kill();
                break;
            case EmptyType:
                snake.step();
                break;
            case RabbitType:
                snake.grow();
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
