#pragma once

#include "model.h"
#include <algorithm>
#include <limits>

namespace sngm {

inline const Rabbit* intuitive_getTarget(const Coord center, const GameModel &model) {
    if (model.getRabbits().empty()) return nullptr;

    unsigned min_dist = std::numeric_limits<unsigned>().max();

    const Rabbit* target = &model.getRabbits().front();
    for (const Rabbit& rabbit: model.getRabbits()) {
        Coord rel = rabbit.pos - center;
        unsigned dist = std::abs(rel.x) + std::abs(rel.y);
        if (dist < min_dist) {
            target = &rabbit;
            min_dist = dist;
        }

    }

    return target;
}

// TODO: randomize direction if it has 2 possible directions
inline Direction intuitive_offsetToDirection(Coord offset, Direction cur_dir) {
    Direction new_dir_x = (offset.x > 0) ? Direction::RIGHT :
                            (offset.x < 0) ? Direction::LEFT : Direction::NONE;

    Direction new_dir_y = (offset.y > 0) ? Direction::UP :
                            (offset.y < 0) ? Direction::DOWN : Direction::NONE;


    Direction new_dir = Direction::NONE;
    if (new_dir_x != Direction::NONE && !isOppositeDirection(new_dir_x, cur_dir)) {
        new_dir = new_dir_x;
    } else if (new_dir_y != Direction::NONE && !isOppositeDirection(new_dir_y, cur_dir)) {
        new_dir = new_dir_y;
    } else {
        if (new_dir_x != Direction::NONE)
            new_dir = directionRotate90(new_dir_x);
        else
            new_dir = directionRotate90(new_dir_y);
    }

    return new_dir;
}

class SnakeBot_Intuitive : public ISnakeBot {
public:
    SnakeBot_Intuitive(Snake& snake): ISnakeBot(snake) {
    }

    void tick(GameModel &model) override {
        if (model.getRabbits().empty()) return;

        Coord head = controlled->body.front();
        const Rabbit* target = intuitive_getTarget(head, model);

        Coord offset = target->pos - head;
        Direction new_dir = intuitive_offsetToDirection(offset, controlled->direction);

        controlled->setDirection(new_dir);
    }

    ~SnakeBot_Intuitive() override = default;

};

class SnakeBot_Intuitive2 : public ISnakeBot {
public:
    SnakeBot_Intuitive2(Snake& snake): ISnakeBot(snake) {
    }

    void tick(GameModel &model) override {
        if (model.getRabbits().empty()) return;

        Coord head = controlled->body.front();
        const Rabbit* target = intuitive_getTarget(head, model);

        Coord offset = target->pos - head;
        Direction new_dir = intuitive_offsetToDirection(offset, controlled->direction);
        // if new head position is in danger, rotating direction
        for (int i = 0; i < 3; i++) {
            Coord new_head = head + new_dir;
            CellType cell = model.checkCoord(new_head);
            if (cell == EmptyType || cell == RabbitType) {
                break;
            }

            new_dir = directionRotate90(new_dir);
        }

        controlled->setDirection(new_dir);
    }

    ~SnakeBot_Intuitive2() override = default;

};



}
