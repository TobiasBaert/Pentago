//
// Created by Tobias Baert on 27/07/2022.
//

#include "Game.h"

#include <iostream>
#include <optional>

std::ostream& operator<<(std::ostream& os, const sf::FloatRect& v) {
    os << "Left: " << v.left << std::endl;
    os << "Top: " << v.top << std::endl;
    os << "Height: " << v.height << std::endl;
    os << "Width: " << v.width << std::endl;
    return os;
}

Game::Game() {
    configureQuadrantShapes();
    configureCellShapes();
    mWindow.setFramerateLimit(60);

    std::cout << mQuadrantShapes[0].getGlobalBounds() << ' ' << mQuadrantShapes[0].getLocalBounds();
}

void Game::configureQuadrantShapes() {
    for (auto& qdShape : mQuadrantShapes) {
        qdShape.setOrigin(QUADRANT_CENTRE);
        qdShape.setFillColor({200,0,0});
        qdShape.setOutlineColor(sf::Color::Black);
        qdShape.setOutlineThickness(-2.f);
    }
}

void Game::configureCellShapes() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            auto& shape = mCircleShapes[i][j];
            shape.setOrigin(CIRCLE_CENTRE);
            shape.setPosition(CELL_SIZE * sf::Vector2f{(float) i - 1, (float) j - 1});
            shape.setFillColor(sf::Color{150,0,0});
            shape.setOutlineColor(sf::Color::Black);
            shape.setOutlineThickness(-2.f);
        }
    }
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

        if (event.type == sf::Event::MouseButtonPressed) {
            std::cout << "A: " << event.mouseButton.x << ' ' << event.mouseButton.y << std::endl;

        }
    }
}

void Game::render() {
    // Set background colour
    sf::Color turnColour = (pBoard->getTurn() == Colour::WHITE ? sf::Color::White : sf::Color::Black);
    mWindow.clear(turnColour);

    renderQuadrant(Quadrant::NORTHWEST);
    renderQuadrant(Quadrant::NORTHEAST);
    renderQuadrant(Quadrant::SOUTHWEST);
    renderQuadrant(Quadrant::SOUTHEAST);

    mWindow.display();
}

void Game::renderQuadrant(const Quadrant q) {
    auto v = sf::Transform(mQuadrantTransforms[to_underlying(q)]).rotate(0);
    mWindow.draw(mQuadrantShapes[to_underlying(q)], v);

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            sf::CircleShape& s = mCircleShapes[row][col];
            s.setFillColor(getSFColorAt(q, row, col));
            mWindow.draw(s, v);
        }
    }
}

sf::Color Game::getSFColorAt(Quadrant q, int row, int col) {
    std::optional<Colour> c = pBoard->getColourAt(q, row, col);
    if (c) return (*c == Colour::WHITE ? sf::Color::White : sf::Color::Black);
    return sf::Color{150,0,0};
}