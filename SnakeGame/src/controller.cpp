#include "controller.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <variant>
#include <fstream>

namespace sngm {

void BotStats::add(Score_t score) {
    count++;
    sum += score;
    sumSq += static_cast<double>(score) * score;

    if (score < min) min = score;
    if (score > max) max = score;

    allScores.push_back(score);
}

double BotStats::stddev() const {
    if (count <= 1) return 0.0;
    double m = mean();
    return std::sqrt((sumSq / count) - (m * m));
}

std::vector<int> BotStats::buildHistogram(int bins) const {
        std::vector<int> hist(bins, 0);
        if (count == 0 || bins <= 0) return hist;

        double range = static_cast<double>(max - min);
        if (range == 0) {
            hist[0] = count;
            return hist;
        }

        for (auto score : allScores) {
            int index = static_cast<int>(
                (score - min) / range * bins
            );

            if (index == bins) index = bins - 1; // крайний случай
            hist[index]++;
        }

        return hist;
    }

void StatisticsManager::addData(const RunStats &stats) {
    for (const auto &[score, bot] : stats) {
        data[bot].add(score);
    }
}



void StatisticsManager::exportStats(std::filesystem::path path) {
    std::ofstream file(path);
    if (!file.is_open()) return;

    file << "BotType,Count,Mean,Min,Max,StdDev";

        for (int i = 0; i < bins; ++i) {
            file << ",Bin" << i;
        }
        file << "\n";

        for (const auto &[bot, stats] : data) {
            auto hist = stats.buildHistogram(bins);

            file << botTypeToString(bot) << ","
                 << stats.count << ","
                 << stats.mean() << ","
                 << stats.min << ","
                 << stats.max << ","
                 << stats.stddev();

            for (auto h : hist) {
                file << "," << h;
            }

            file << "\n";
        }

        file.close();
}


void GameController::run() {
    using namespace std::chrono_literals;


    while (!exit_request) {
        auto current_time = clock.now();

        while (std::optional<GameEvent> event = view.pollEvent()) {
            // std::cout << "processing event with code " << static_cast<int>() << "\n";
            processEvent(*event);
        }

        if (next_tick <= current_time && !is_paused) {
            model.tickStep();
            next_tick = current_time + tickPeriod;

            if (model.aliveSnakes() == 0 && tournament_mode) {
                stat_manager.addData(model.exportScores());
                model.restart();
            }
        }

        if (next_render <= current_time) {
            view.render(model);
            next_render = current_time + renderPeriod;
        }

        // std::this_thread::sleep_until(std::min(next_render, next_tick));
    }

    if (tournament_mode) {
        stat_manager.exportStats(STATS_PATH);
    }


}

void GameController::processEvent(const GameEvent& event) {
    if (event.is<KeyEvent>()) {
        KeyEvent key = event.get<KeyEvent>();
        switch (key) {
            case KeyEvent::EXIT:
                exit_request = true;
                break;
            case KeyEvent::P1_UP:
                model.setPlayerSnakeDir(0, Direction::UP);
                break;
            case KeyEvent::P1_DOWN:
                model.setPlayerSnakeDir(0, Direction::DOWN);
                break;
            case KeyEvent::P1_LEFT:
                model.setPlayerSnakeDir(0, Direction::LEFT);
                break;
            case KeyEvent::P1_RIGHT:
                model.setPlayerSnakeDir(0, Direction::RIGHT);
                break;
            case KeyEvent::P2_UP:
                model.setPlayerSnakeDir(1, Direction::UP);
                break;
            case KeyEvent::P2_DOWN:
                model.setPlayerSnakeDir(1, Direction::DOWN);
                break;
            case KeyEvent::P2_LEFT:
                model.setPlayerSnakeDir(1, Direction::LEFT);
                break;
            case KeyEvent::P2_RIGHT:
                model.setPlayerSnakeDir(1, Direction::RIGHT);
                break;
            case KeyEvent::PAUSE:
                is_paused = !is_paused;
                break;
            case sngm::KeyEvent::RESTART:
                model.restart();
                break;
            default:
                break;
        }
    } else if (event.is<WinchEvent>()) {
        WinchEvent ws = event.get<WinchEvent>();
        model.resize(ws.width, ws.height);
    }
}



}
