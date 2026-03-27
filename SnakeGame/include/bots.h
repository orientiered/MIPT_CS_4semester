#pragma once

#include "model.h"
#include <algorithm>
#include <limits>

namespace sngm {

class SnakeBot_Intuitive : public ISnakeBot {
public:
    SnakeBot_Intuitive(Snake& snake): ISnakeBot(snake) {
    }

    void tick(GameModel &model) override {
        if (model.getRabbits().empty()) return;

        Coord head = controlled->body.front();

        unsigned min_dist = std::numeric_limits<unsigned>().max();

        const Rabbit* target = &model.getRabbits().front();
        for (const Rabbit& rabbit: model.getRabbits()) {
            Coord rel = rabbit.pos - head;
            unsigned dist = std::abs(rel.x) + std::abs(rel.y);
            if (dist < min_dist) {
                target = &rabbit;
                min_dist = dist;
            }

        }

        Coord offset = target->pos - head;
        Direction new_dir_x = (offset.x > 0) ? Direction::RIGHT :
                              (offset.x < 0) ? Direction::LEFT : Direction::NONE;

        Direction new_dir_y = (offset.y > 0) ? Direction::UP :
                              (offset.y < 0) ? Direction::DOWN : Direction::NONE;


        Direction new_dir = Direction::NONE;
        if (new_dir_x != Direction::NONE && !isOppositeDirection(new_dir_x, controlled->direction)) {
            new_dir = new_dir_x;
        } else if (new_dir_y != Direction::NONE && !isOppositeDirection(new_dir_y, controlled->direction)) {
            new_dir = new_dir_y;
        } else {
            if (new_dir_x != Direction::NONE)
                new_dir = directionRotate90(new_dir_x);
            else
                new_dir = directionRotate90(new_dir_y);
        }

        controlled->setDirection(new_dir);
    }

    ~SnakeBot_Intuitive() override = default;

};


}
