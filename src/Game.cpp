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
            auto x = static_cast<float>(event.mouseButton.x);
            auto y = static_cast<float>(event.mouseButton.y);
            auto q = quadrantFromPosition({x,y});
            auto c = cellCoordsFromPosition({x,y});
            std::cout << "A: " << x << ' ' << y << std::endl;
            std::cout << "Q: " << (q ? std::to_string(to_underlying(*q)) : "Nope") << std::endl;
            std::cout << "C1: " << (c ? std::to_string(c->first) : "Nope") << std::endl;
            std::cout << "C2: " << (c ? std::to_string(c->second) : "Nope") << std::endl;
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

Game::OptionalQuadrant Game::quadrantFromPosition(sf::Vector2f position) {
    static constexpr std::array<Quadrant, 4> qds {
        Quadrant::NORTHWEST, Quadrant::NORTHEAST,
        Quadrant::SOUTHWEST, Quadrant::SOUTHEAST
    };

    for (int i = 0; i < 4; i++) {
        auto p = mQuadrantTransforms[i].getInverse().transformPoint(position);
        if (mQuadrantShape.withinGlobalBounds(p)) return qds[i];
    }

    return {std::nullopt};
}

Game::OptionalIntPair Game::cellCoordsFromPosition(sf::Vector2f position) {
    auto e = quadrantFromPosition(position);
    if (!e) return {std::nullopt};

    const auto quadrantTransform = mQuadrantTransforms[to_underlying(*e)];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            auto p = (quadrantTransform * mCellTransforms[i][j]).getInverse().transformPoint(position);
            if (Util::distanceLessThan(p, {0.f, 0.f}, CIRCLE_RADIUS)) return std::pair(i,j);
        }
    }
    return {std::nullopt};
}
