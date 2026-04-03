#include "graphic_view.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>

namespace sngm {

struct GraphicView::Impl {
    sf::RenderWindow window;

	sf::CircleShape shape;
    float pixels_per_cell = 50;

    Impl() {
        window.create(sf::VideoMode( { 500, 500 } ), "SFML works!" );
        shape.setRadius( 100.f );
    	shape.setFillColor( sf::Color::Green );

    }

    sf::Vector2f coordToScreen(Coord coord) {
        return {coord.x * pixels_per_cell, coord.y * pixels_per_cell};
    }

    void render(const GameModel& model) {
        if (!window.isOpen()) return;

        window.clear();

        for (const Rabbit& rabbit: model.getRabbits()) {
            sf::CircleShape rab_shape(50);
        	shape.setFillColor( sf::Color::Green );
            rab_shape.setPosition(coordToScreen(rabbit.pos));

            window.draw(shape);
        }
		// window.draw( shape );

		window.display();
    }

    std::optional<GameEvent> pollEvent() {
        const std::optional event = window.pollEvent();

        if (event->is<sf::Event::Closed>() ) {
            std::cerr << "Close event\n";
            // return GameEvent{KeyEvent::EXIT};
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
        }

        return std::nullopt;
    }

    ~Impl() {
        window.close();
    }
};

void GraphicView::render(const GameModel& model) {
    impl_->render(model);

}

std::optional<GameEvent> GraphicView::pollEvent() {
    return impl_->pollEvent();
}

GraphicView::GraphicView(uint32_t screen_width, uint32_t screen_height): impl_(std::make_unique<Impl>()) {}
GraphicView::~GraphicView() = default;


}
