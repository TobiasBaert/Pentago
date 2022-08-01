//
// Created by Tobias Baert on 27/07/2022.
//

#include "Game.h"

#include <iostream>
#include <optional>

Game::Game() {
    configureQuadrantShapes();
    configureCellShapes();
    mWindow.setFramerateLimit(60);
}

void Game::configureQuadrantShapes() {
    for (auto& qdShape : mQuadrantShapes) {
        qdShape.setOrigin({QUADRANT_SIZE / 2, QUADRANT_SIZE / 2});
        qdShape.setFillColor({200,0,0});
        qdShape.setOutlineColor(sf::Color::Black);
        qdShape.setOutlineThickness(2.f);
    }
}

void Game::configureCellShapes() {
    for (auto& row : mCircleShapes) {
        for (auto& shape : row) {
            shape.setOrigin({CIRCLE_RADIUS, CIRCLE_RADIUS});
            shape.setFillColor(sf::Color{150,0,0});
            shape.setOutlineColor(sf::Color::Black);
            shape.setOutlineThickness(2.f);
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
            std::cout << event.mouseButton.x << ' ' << event.mouseButton.y << std::endl;
        }
    }
}

void Game::render() {
    // Set background colour
    sf::Color turnColour = (pBoard->getTurn() == Colour::WHITE ? sf::Color::White : sf::Color::Black);
    mWindow.clear(turnColour);

    static const sf::Vector2f screenCentreOffset {SCREEN_SIZE / 2.f, SCREEN_SIZE / 2.f};
    renderAllQuadrants(sf::Transform().translate(screenCentreOffset));

    mWindow.display();
}

void Game::renderAllQuadrants(const sf::Transform t) {
    static const float qdCentreOffset {QUADRANT_SIZE / 2 + INTRA_QUADRANT_MARGIN};
    renderSingleQuadrant(Quadrant::NORTHWEST, sf::Transform(t).translate({-qdCentreOffset, -qdCentreOffset}));
    renderSingleQuadrant(Quadrant::NORTHEAST, sf::Transform(t).translate({ qdCentreOffset, -qdCentreOffset}));
    renderSingleQuadrant(Quadrant::SOUTHWEST, sf::Transform(t).translate({-qdCentreOffset,  qdCentreOffset}));
    renderSingleQuadrant(Quadrant::SOUTHEAST, sf::Transform(t).translate({ qdCentreOffset,  qdCentreOffset}));
}

void Game::renderSingleQuadrant(const Quadrant q, const sf::Transform t) {
    auto v = sf::Transform(t).rotate(0);
    mWindow.draw(mQuadrantShapes[to_underlying(q)], v);
    renderAllCellsForQuadrant(q, v);
}

void Game::renderAllCellsForQuadrant(const Quadrant q, const sf::Transform t) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            auto u = sf::Transform(t).translate(CELL_SIZE * sf::Vector2f{(float) col - 1, (float) row - 1});
            renderSingleCellForQuadrant(q, row, col, u);
        }
    }
}

void Game::renderSingleCellForQuadrant(const Quadrant q, int row, int col, const sf::Transform t) {
    sf::CircleShape& s = mCircleShapes[row][col];
    s.setFillColor(getSFColorAt(q, row, col));
    mWindow.draw(s, t);
}

sf::Color Game::getSFColorAt(Quadrant q, int row, int col) {
    std::optional<Colour> c = pBoard->getColourAt(q, row, col);
    if (c) return (*c == Colour::WHITE ? sf::Color::White : sf::Color::Black);
    return sf::Color{150,0,0};
}