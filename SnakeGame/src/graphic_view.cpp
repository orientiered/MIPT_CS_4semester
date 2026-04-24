#include "graphic_view.h"
#include "coords.h"
#include "game_event.h"
#include "snake.h"
#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>


namespace sngm {




static sf::Color hsv(int hue, float sat, float val) {
    hue %= 360;
    while (hue < 0) hue += 360;

    sat = std::clamp(sat, 0.f, 1.f);
    val = std::clamp(val, 0.f, 1.f);

    int h = hue / 60;
    float f = (static_cast<float>(hue) / 60.0f) - h;
    float p = val * (1.0f - sat);
    float q = val * (1.0f - (sat * f));
    float t = val * (1.0f - (sat * (1.0f - f)));

    float r, g, b;
    switch (h) {
        case 0:  r = val; g = t;   b = p;   break;
        case 1:  r = q;   g = val; b = p;   break;
        case 2:  r = p;   g = val; b = t;   break;
        case 3:  r = p;   g = q;   b = val; break;
        case 4:  r = t;   g = p;   b = val; break;
        default: r = val; g = p;   b = q;   break;
    }

    return sf::Color(
        static_cast<uint8_t>(r * 255),
        static_cast<uint8_t>(g * 255),
        static_cast<uint8_t>(b * 255)
    );
}


struct SnakeSprites {
    sf::Sprite linear;
    sf::Sprite turn;
    sf::Sprite tail;
    sf::Sprite head;

    int sprite_size = 255;

    SnakeSprites(sf::Texture &atlas): 
        linear(atlas), tail(atlas), turn(atlas), head(atlas) {
                
        sf::Vector2i pos(0,0);


        auto setRectAndOrigin = [&](sf::Sprite &sprite, sf::Vector2i size) {
            sprite.setTextureRect(sf::IntRect(pos, size));
            sprite.setOrigin(sf::Vector2f(size)*0.5f);

            pos.x += size.x;
            pos.x++; // offset by one pixel
        };

        sf::Vector2i linear_size(161, 255);
        setRectAndOrigin(linear, linear_size);
        
        sf::Vector2i turn_size(363-162, 215);
        setRectAndOrigin(turn, turn_size);
        turn.setOrigin({161.f/2, 161.f/2});

        sf::Vector2i tail_size(161, 255);
        setRectAndOrigin(tail, tail_size);

        pos.x++;
        sf::Vector2i head_size(786-529, 334);
        setRectAndOrigin(head, head_size);

    }
};

struct GraphicView::Impl {
    sf::RenderWindow window;

    float pixels_per_cell = 40;
    sf::FloatRect reference_field_rect;
    sf::FloatRect game_field_rect;

    sf::Font font;

    sf::Texture atlas;

    SnakeSprites snake_sprites;

    bool initial_resize = true;

    Impl(uint32_t screen_width, uint32_t screen_height ):
        snake_sprites(atlas) 
    {
        window.create(sf::VideoMode( { screen_width, screen_height } ), "Snake game!" );

        reference_field_rect.position = {0.1, 0.05};
        reference_field_rect.size = {0.8, 0.90};

        game_field_rect.position = {0.1, 0.05};
        game_field_rect.size = {0.8, 0.90};

        if (!font.openFromFile(FONT_PATH)) {
            throw std::runtime_error("Failed to open font " + FONT_PATH.string());
        }

        if (!atlas.loadFromFile(ATLAS_PATH)) {
            throw std::runtime_error("Failed to open texture atlas " + ATLAS_PATH.string());
        }

    }

    sf::Vector2f scaleRelToScreen(sf::Vector2f pos) {
        sf::Vector2f ws = sf::Vector2f(window.getSize());

        return ws.componentWiseMul(pos);
    }

    sf::Vector2f coordToScreen(Coord coord) {
        sf::Vector2f ws = sf::Vector2f(window.getSize());

        sf::Vector2f pos = game_field_rect.position;
        sf::Vector2f size = game_field_rect.size;

        float x = pos.x * ws.x + coord.x * pixels_per_cell;
        float y = (pos.y + size.y) * ws.y - coord.y * pixels_per_cell - pixels_per_cell;
        return {x, y};
    }

    sf::Vector2f coordToScreenCentered(Coord coord) {
        return coordToScreen(coord) + sf::Vector2f{pixels_per_cell, pixels_per_cell}*0.5f;
    }

    void drawRabbit(const Rabbit& rabbit);

    void drawSnake(const Snake& snake, sf::Color body_col, sf::Color head_col) {
        if (!snake.isAlive) return;

        Coord prev_coord;
        Coord next_coord = snake.body.front();

        sf::Sprite* segment = &snake_sprites.head;
        float scale = pixels_per_cell / snake_sprites.sprite_size;

        auto snake_it = snake.body.begin();
        int snake_size = snake.body.size();
        for (int i = 0; i < snake_size; i++, snake_it++) {

            Coord cur = next_coord;

            if (i < snake.body.size() - 1) {
                snake_it++;
                next_coord =  *snake_it;
                snake_it--;
            }

            Coord dif1 = cur - prev_coord,
                    dif2 = next_coord - cur;

            prev_coord = cur;

            sf::Vector2f pos = coordToScreenCentered(cur);

            float rotation = 0;
            if (i == 0) {
                // head
                continue; // head must be drawn last
            } else if (i == (snake_size - 1)) {
                //tail
                segment = &snake_sprites.tail;
                rotation =  (dif1.x > 0) ? -90 :
                            (dif1.x < 0) ? +90 :
                            (dif1.y > 0) ? +180 : 
                            (dif1.y < 0) ? 0 : 0;

            } else if (dif1 == dif2) {
                // linear
                segment = &snake_sprites.linear;
                rotation = (dif1.x == 0) ? 0 : 90;
            } else {
                // turn
                segment = &snake_sprites.turn;
                bool l = (dif1.x > 0) || (dif2.x < 0),
                        r = (dif1.x < 0) || (dif2.x > 0),
                        u = (dif1.y < 0) || (dif2.y > 0),
                        d = (dif1.y > 0) || (dif2.y < 0);

                rotation =  (d && l) ? 90  :
                            (l && u) ? 180 :
                            (u && r) ? -90 :
                            (r && d) ? 0 : 0; 
                    
            }

            segment->setPosition(pos);
            segment->setScale({scale, scale});
            segment->setColor(body_col);
            segment->setRotation(sf::degrees(rotation));

            window.draw(*segment);
        }

        segment = &snake_sprites.head;
        segment->setColor(head_col);
        segment->setRotation(sf::degrees(180) + sf::degrees(directionToDegree(snake.direction)));
        segment->setScale({scale, scale});
        // sf::Vector2f head_offset = sf::Vector2f(sf::Vector2i{snake_sprites.sprite_size,snake_sprites.sprite_size})*0.75f*scale;
        // sf::Vector2f head_offset = {0.f, 0.f};
        segment->setPosition(coordToScreenCentered(snake.body.front()));

        window.draw(*segment);

    }

    // color palette from 0 to snake_palette_len - 1
    const int snake_palette_len = 10; 
    std::pair<sf::Color, sf::Color> getSnakeColors(int i) {

        return {hsv(360 * i / snake_palette_len, 0.8, 0.5),
               hsv(360 * i / snake_palette_len, 0.8, 0.7)};    
    }

    void drawScores(const GameModel& model) {
        const std::deque<Snake> snakes = model.getSnakes();
        for (int i = 0 ; i < snakes.size(); i++) {
            SnakeId id = snakes[i].id_;

            bool is_bot = i >= model.controllable_snakes;

            std::string text = (is_bot ? "Bot" : "Player") + std::to_string(i+1) + 
                    " score: " + std::to_string(model.getScore(id));

            sf::Text snake_text(font, text);
            auto snake_col = getSnakeColors(i);
            snake_text.setFillColor(snake_col.second);
            snake_text.setPosition({0, i*snake_text.getGlobalBounds().size.y});

            window.draw(snake_text);
        }
    }

    void render(const GameModel& model) {
        if (!window.isOpen()) return;

        window.clear();

        sf::RectangleShape border(scaleRelToScreen(game_field_rect.size));
        border.setPosition(scaleRelToScreen(game_field_rect.position));
        border.setOutlineThickness(3);
        border.setOutlineColor(sf::Color::White);
        border.setFillColor(sf::Color::Black);

        window.draw(border);

        if (!model.isValid()) {
            std::string_view err_str = model.getErrorString();
            sf::Text err_text(font, err_str.data());
            err_text.setFillColor(sf::Color::Red);
            err_text.setPosition(scaleRelToScreen(game_field_rect.getCenter()) - err_text.getGlobalBounds().size * 0.5f );
            window.draw(err_text);
        } else {
            for (const Rabbit& rabbit: model.getRabbits()) {
                drawRabbit(rabbit);
            }

            // model.controllable_snakes
            for (int i = 0; i < model.getSnakes().size(); i++) {
                auto snake_col = getSnakeColors(i);
                drawSnake(model.getSnakes()[i], snake_col.first, snake_col.second);
            }

            drawScores(model);
        }

		window.display();
    }

    WinchEvent recalc_field_size() {
        sf::Vector2u ws = window.getSize();
        sf::Vector2f size = reference_field_rect.size;

        int width = static_cast<int>(ws.x * size.x / pixels_per_cell);
        int height = static_cast<int>(ws.y * size.y / pixels_per_cell);
        std::cerr << "Resize: " << width << " " << height << "\n";

        game_field_rect.size = {width * pixels_per_cell / ws.x, height * pixels_per_cell / ws.y};
        game_field_rect.position = (sf::Vector2f{1.f, 1.f} - game_field_rect.size) * 0.5f;

        return WinchEvent{width, height};
    }

    std::optional<GameEvent> pollEvent() {
        if (initial_resize) {
            initial_resize = false;
            return GameEvent{recalc_field_size()};
        }

        const std::optional<sf::Event> event = window.pollEvent();
        if (!event) return std::nullopt;

        if (event->is<sf::Event::Closed>() ) {
            std::cerr << "Close event\n";
            return GameEvent{KeyEvent::EXIT};
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            using Scancode = sf::Keyboard::Scancode;
            std::map<Scancode, KeyEvent> key_map = {
                {Scancode::Escape, KeyEvent::EXIT    },
                {Scancode::W,      KeyEvent::P1_UP   },
                {Scancode::A,      KeyEvent::P1_LEFT },
                {Scancode::S,      KeyEvent::P1_DOWN },
                {Scancode::D,      KeyEvent::P1_RIGHT},
                {Scancode::Up,     KeyEvent::P2_UP   },
                {Scancode::Left,   KeyEvent::P2_LEFT },
                {Scancode::Down,   KeyEvent::P2_DOWN },
                {Scancode::Right,  KeyEvent::P2_RIGHT},
                {Scancode::P,      KeyEvent::PAUSE   },
                {Scancode::R,      KeyEvent::RESTART }
            };
            auto event_it = key_map.find(keyPressed->scancode);
            if (event_it != key_map.end())
                return GameEvent{event_it->second};
        } else if (const auto *resized = event->getIf<sf::Event::Resized>()) {
            sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
            window.setView(sf::View(visibleArea));

            return GameEvent{recalc_field_size()};
        }

        return std::nullopt;
    }

    ~Impl() {
        window.close();
    }
};

void GraphicView::Impl::drawRabbit(const Rabbit& rabbit) {
    sf::CircleShape rab_shape(pixels_per_cell/2);
    rab_shape.setFillColor(sf::Color::Red);
    rab_shape.setPosition(coordToScreen(rabbit.pos));

    window.draw(rab_shape);
}

void GraphicView::render(const GameModel& model) {
    impl_->render(model);

}

std::optional<GameEvent> GraphicView::pollEvent() {
    return impl_->pollEvent();
}

GraphicView::GraphicView(uint32_t screen_width, uint32_t screen_height): impl_(std::make_unique<Impl>(screen_width, screen_height)) {}
GraphicView::~GraphicView() = default;


}
