#include "bots.h"

namespace sngm {

Direction intuitive_offsetToDirection(
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

Direction findPathBFS(const GameModel& model, Coord start) {
    std::queue<Coord> q;
    std::map<Coord, Coord> parent;

    q.push(start);
    parent[start] = start;

    std::vector<Direction> dirs = {
        Direction::UP, Direction::DOWN,
        Direction::LEFT, Direction::RIGHT
    };

    Coord target = start;
    
    while (!q.empty()) {
        Coord cur = q.front(); q.pop();

        // stop when find first rabbit
        CellType cur_type = model.checkCoord(cur).type;
        if (cur_type == RabbitType) {
            target = cur;
            break;
        }

        for (auto d : dirs) {
            Coord next = cur + d;

            if (parent.count(next)) continue;

            // only empty or rabbits cells are traversible 
            auto cell = model.checkCoord(next).type;
            if (cell != EmptyType && cell != RabbitType) continue;

            parent[next] = cur;
            q.push(next);
        }
    }

    if (!parent.count(target)) {
        return Direction::NONE; // путь не найден
    }

    // восстановление первого шага
    Coord cur = target;
    while (parent[cur] != start) {
        cur = parent[cur];
    }

    Coord step = cur - start;

    return intuitive_offsetToDirection(step, Direction::NONE);
}

}