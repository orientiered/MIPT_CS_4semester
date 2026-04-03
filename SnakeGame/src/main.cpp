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
    arg_parser.add_option("-p,--players", player_count, "Number of human-controlled snakes (players)");

    int bot_count = 0;
    arg_parser.add_option("-b,--bots", bot_count, "Number of bot-controlled snakes");

    int bot_type = 0;
    arg_parser.add_option("-t,--bot-type", bot_type, "Bots type: 0 -> dumb, 1->medium");

    CLI11_PARSE(arg_parser, argc, argv);

    if (player_count < 0) {
        std::cerr << "Invalid number of player snakes: defaulting to 1\n";
        player_count = 1;
    }

    if (bot_count < 0 ) {
        std::cerr << "Invalid number of bot snakes: defaulting to 0\n";
    }

    if (bot_type < 0 || bot_type > sngm::SNAKE_BOT_MEDIUM) {
        std::cerr << "Invalid bot type, defaulting to DUMB\n";
        bot_type = 0;
    }

    // Game launch

    auto bot_pattern = std::vector<sngm::BotType>(bot_count, sngm::BotType(bot_type));
    sngm::GameModel model(40, 30, player_count, bot_count, bot_pattern);

    // sngm::AsciiView view;
    sngm::GraphicView view;

    sngm::GameController controller(model, view);

    controller.run();

    return 0;
}
