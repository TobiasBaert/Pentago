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
    pBoard->placeAt(Colour::WHITE, 2, 3);
}

void Game::configureQuadrantShapes() {
    mQuadrantShape.setOrigin(QUADRANT_CENTRE);
    mQuadrantShape.setFillColor({200,0,0});
    mQuadrantShape.setOutlineColor(sf::Color::Black);
    mQuadrantShape.setOutlineThickness(-2.f);
}

void Game::configureCellShapes() {
    mCellShape.setOrigin(CIRCLE_CENTRE);
    mCellShape.setFillColor(sf::Color{150,0,0});
    mCellShape.setOutlineColor(sf::Color::Black);
    mCellShape.setOutlineThickness(-2.f);
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
    mWindow.draw(mQuadrantShape, v);

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            mCellShape.setFillColor(getSFColorAt(q, row, col));
            mWindow.draw(mCellShape, v * mCellTransforms[row][col]);
        }
    }
}

sf::Color Game::getSFColorAt(Quadrant q, int row, int col) {
    std::optional<Colour> c = pBoard->getColourAt(q, row, col);
    if (c) return (*c == Colour::WHITE ? sf::Color::White : sf::Color::Black);
    return sf::Color{150,0,0};
}

sf::Transform Game::translation(sf::Vector2f t) {
    return sf::Transform().translate(t);
}
