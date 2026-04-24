#pragma once

#include "game_event.h"
#include "model.h"
#include "view.h"
#include <chrono>
#include <filesystem>

namespace sngm {

using namespace std::chrono_literals;

static inline std::filesystem::path STATS_PATH = "stats.csv";

struct BotStats {
    int count = 0;
    double sum = 0.0;
    double sumSq = 0.0;
    Score_t min = std::numeric_limits<Score_t>::max();
    Score_t max = std::numeric_limits<Score_t>::min();

    std::vector<Score_t> allScores;

    void add(Score_t score);

    double mean() const {
        return count ? sum / count : 0.0;
    }

    double stddev() const;

    std::vector<int> buildHistogram(int bins) const;

};

class StatisticsManager {
private:
    std::map<BotType, BotStats> data;
    int bins;
public:
    explicit StatisticsManager(int histogramBins = 10)
        : bins(histogramBins) {}

    void exportStats(std::filesystem::path path);
    void addData(const RunStats &stats);
};

class GameController {
private:
    GameModel &model;
    IView &view;

    std::chrono::steady_clock clock;

    using time_point_t = typeof(clock.now());

    std::chrono::milliseconds tickPeriod = 200ms;
    std::chrono::milliseconds renderPeriod = 33ms;

    time_point_t next_tick;
    time_point_t next_render;

    bool is_paused = false;
    bool exit_request = false;

    StatisticsManager stat_manager;
    bool tournament_mode = false;

public:
    void setTickPeriod(std::chrono::milliseconds period) {
        tickPeriod = std::max(period, 5ms);
    }

    GameController(GameModel& _model, IView& _view, bool tournament):
        model(_model), view(_view), tournament_mode(tournament) {
            if (tournament_mode) {
                setTickPeriod(5ms);
            }

            next_tick = clock.now();
            next_render = clock.now();

        }

    void run();

private:
    void processEvent(const GameEvent& event);

};


}

