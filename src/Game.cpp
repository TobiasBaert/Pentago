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
}

void Game::setQuadrantToRenderLast(Quadrant q) {
    auto p = std::find(mQuadrantRenderOrder.begin(), mQuadrantRenderOrder.end(),q); // find
    std::swap(*p, mQuadrantRenderOrder[3]); // swap
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
            auto [q, c] = getCoordinateTupleFromPosition({x,y});
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

    for (auto q : mQuadrantRenderOrder) {
        renderQuadrant(q);
    }

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

Game::OptionalQuadrant Game::getQuadrantFromPosition(sf::Vector2f p, sf::Vector2f& pInQuadrantLocalCoordSystem) const {
    // determine the quadrant by:
    // 1) applying the reverse transformation to the centre of the quadrant to the given position. This yields the
    // given position relative to the centre of the quadrant.
    // 2) checking if the transformed position is within the global bounds of the quadrant.
    for (int i = 0; i < 4; i++) {
        pInQuadrantLocalCoordSystem = mQuadrantTransforms[i].getInverse().transformPoint(p);
        if (mQuadrantShape.withinGlobalBounds(pInQuadrantLocalCoordSystem)) return Quadrant(i);
    }

    return {std::nullopt};
}

Game::OptionalIntPair Game::getCellCoordsFromPosition(sf::Vector2f pInQuadrantLocalCoordSystem) const {
    // determine the cell indices by:
    // 1) transforming the given position, which should be relative to the centre of the quadrant, by applying the
    // inverse transformation from the centre of the quadrant to the centre of a cell. This yields the given position
    // relative to the centre of the cell.
    // 2) checking if the distance between the transformed position and the origin, i.e., centre of the circle shape,
    // is less than the radius of said circle shape.
    sf::Vector2f pInCellLocalCoordSystem;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            pInCellLocalCoordSystem = mCellTransforms[i][j].getInverse().transformPoint(pInQuadrantLocalCoordSystem);
            if (Util::distanceLessOrEqualTo(pInCellLocalCoordSystem, {0.f, 0.f}, CIRCLE_RADIUS)) return {{i, j}};
        }
    }
    return {std::nullopt};
}

Game::CoordinateTuple Game::getCoordinateTupleFromPosition(sf::Vector2f p) const {
    OptionalQuadrant q;
    OptionalIntPair coords;

    sf::Vector2f pInQuadrantLocalCoordSystem;
    q = getQuadrantFromPosition(p, pInQuadrantLocalCoordSystem);
    if (q) coords = getCellCoordsFromPosition(pInQuadrantLocalCoordSystem);

    return CoordinateTuple{q, coords};
}
