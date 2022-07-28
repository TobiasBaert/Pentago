//
// Created by Tobias Baert on 27/07/2022.
//

#include "Game.h"
#include "RoundedRectangle.hpp"

#include <iostream>

Game::Game() : mWindow(sf::VideoMode(1200,1200), "Pentago", sf::Style::Close | sf::Style::Titlebar) {
    mWindow.setFramerateLimit(60);
}

void Game::run() {
    while (mWindow.isOpen()) {
        processEvents();
        render();
    }

}

void Game::processEvents() {
    sf::Event event{};
    while (mWindow.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            mWindow.close();

        if (event.type == sf::Event::MouseButtonPressed)
            std::cout << event.mouseButton.x << ' ' << event.mouseButton.y << std::endl;
    }
}

void Game::render() {
    mWindow.clear(sf::Color::Black);
    RoundedRectangle<5> rectangle(sf::Vector2f{200,200}, 20);
    rectangle.setFillColor(sf::Color::Red);
    mWindow.draw(rectangle);
    mWindow.display();
}
