//
// Created by Tobias Baert on 27/07/2022.
//

#include "Game.h"

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

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {

            const float xOffset = 200 * (float) i;
            const float yOffset = 200 * (float) j;

            // draw rectangle
            sf::RectangleShape redBackgroundSquare{{180, 180}};
            redBackgroundSquare.setPosition({xOffset + 10, yOffset + 10});
            redBackgroundSquare.setFillColor(sf::Color::Red);
            mWindow.draw(redBackgroundSquare);

            // draw circle
            sf::CircleShape circle{80};
            circle.setPosition(xOffset + 20, yOffset + 20);
            circle.setFillColor({128,128,128});

            std::optional<Colour> col = pBoard->colourAt(i,j);
            if (col) {
                circle.setFillColor(to_underlying(*col) ? sf::Color::White : sf::Color::Black);
            }

            mWindow.draw(circle);
        }
    }

    mWindow.display();
}
