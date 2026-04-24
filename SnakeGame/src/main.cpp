#include <iostream>

#include "ascii_view.h"
#include "controller.h"
#include "graphic_view.h"
#include "model.h"
#include "snake.h"

#include "3rd-party/CLI11.hpp"

int main(int argc, const char *argv[]) {

    // CLI
    CLI::App arg_parser{"Snake game"};

    int player_count = 1;

    bool text_view = false;

    arg_parser.add_flag("-a,--ascii", text_view, "Use text view instead of graphics");

    arg_parser.add_option("-p,--players", player_count, "Number of human-controlled snakes (players)");

    std::string bot_config;
    arg_parser.add_option("-b,--bots", bot_config, "Syntax: <bot_name>:<count>  Ex: --bots bfs:3,medium:2");

    bool tournament_mode = false;
    arg_parser.add_flag("--tournament", tournament_mode, "Tournament mode: run multiple games");

    int tick_period = 200;
    arg_parser.add_option("--tick-period", tick_period, "Game tick period in ms");

    CLI11_PARSE(arg_parser, argc, argv);


    if (player_count < 0) {
        std::cerr << "Invalid number of player snakes: defaulting to 1\n";
        player_count = 1;
    }


    auto bot_pattern = std::vector<sngm::BotType>();

    {
        std::map<std::string, sngm::BotType> bot_map = {
            {"dumb", sngm::SNAKE_BOT_DUMB},
            {"medium", sngm::SNAKE_BOT_MEDIUM},
            {"bfs", sngm::SNAKE_BOT_BFS}
        };

        std::stringstream ss(bot_config);
        std::string token;

        while (std::getline(ss, token, ',')) {
            auto colon = token.find(':');
            if (colon == std::string::npos) continue;

            std::string name = token.substr(0, colon);
            int count = std::stoi(token.substr(colon + 1));

            if (!bot_map.count(name)) {
                std::cerr << "Unknown bot: " << name << "\n";
                continue;
            }

            for (int i = 0; i < count; ++i) {
                bot_pattern.push_back(bot_map[name]);
            }
        }
    }

    // Game launch
    sngm::GameModel model(40, 30, player_count, bot_pattern.size(), bot_pattern);

    std::unique_ptr<sngm::IView> view;

    if (text_view) {
        view = std::make_unique<sngm::AsciiView>();

    } else {
        view = std::make_unique<sngm::GraphicView>();
    }

    sngm::GameController controller(model, *view, tournament_mode);

    controller.setTickPeriod(std::chrono::milliseconds(tick_period));
    controller.run();

    return 0;
}
