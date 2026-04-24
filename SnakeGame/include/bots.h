#pragma once

#include "model.h"
#include <algorithm>
#include <limits>
#include <random>

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


inline Direction intuitive_offsetToDirection(
    Coord offset,
    Direction cur_dir
) {
    static std::mt19937 rng(std::random_device{}());

    std::vector<Direction> candidates;

    Direction dx = (offset.x > 0) ? Direction::RIGHT :
                   (offset.x < 0) ? Direction::LEFT : Direction::NONE;

    Direction dy = (offset.y > 0) ? Direction::UP :
                   (offset.y < 0) ? Direction::DOWN : Direction::NONE;

    if (dx != Direction::NONE && !isOppositeDirection(dx, cur_dir))
        candidates.push_back(dx);

    if (dy != Direction::NONE && !isOppositeDirection(dy, cur_dir))
        candidates.push_back(dy);

    // Choosing random appropriate direction
    if (!candidates.empty()) {
        std::uniform_int_distribution<> dist(0, candidates.size() - 1);
        return candidates[dist(rng)];
    }

    // fallback 
    if (dx != Direction::NONE)
        return directionRotate90(dx);
    if (dy != Direction::NONE)
        return directionRotate90(dy);

    return cur_dir;
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
            CellType cell = model.checkCoord(new_head).type;
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
