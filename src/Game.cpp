//
// Created by Tobias Baert on 27/07/2022.
//

#include "Game.h"

#include <iostream>
#include <memory>
#include <optional>

using namespace Enums;

Game::Game() {
    configureQuadrantShapes();
    configureCellShapes();
    configureGameEndOverlay();
    configureGameEndFontAndText();
    mWindow.setFramerateLimit(60);
}

float Game::getRotationFor(Quadrant q) const {
    if (!mRotatingQuadrant) return 0.f;
    auto [r,f] = *mRotatingQuadrant;
    return (q == r ? f : 0.f);
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

void Game::configureGameEndOverlay() {
    mGameEndOverlay.setFillColor({0,0,0,32});
}

void Game::configureGameEndFontAndText() {
    if (!mGameEndFont.loadFromFile("../resources/fonts/Comic_Sans_MS.ttf")) throw std::runtime_error("Can't load font");
    mGameEndText.setFont(mGameEndFont);
    mGameEndText.setFillColor(sf::Color::White);
    mGameEndText.setOutlineColor(sf::Color::Black);
    mGameEndText.setOutlineThickness(10.f);
    mGameEndText.setCharacterSize(200);
    mGameEndText.setStyle(sf::Text::Regular);
}

void Game::run() {
    while (mWindow.isOpen()) {
        processEvents();
        processInputs();
        render();
    }
}

void Game::processEvents() {
    sf::Event event{};
    while (mWindow.pollEvent(event))
        switch (event.type) {
            case sf::Event::Closed:
                mWindow.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::R)
                    pBoard->reset();
        default: break;
        }
}

void Game::processInputs() {
    if (pBoard->getGamePhase() == Enums::GamePhase::PLAYING)
        mSavedStates[to_underlying(pBoard->getTurnPhase())]->processInputs();
}

void Game::render() {
    // Set background colour
    mWindow.clear(toSFColor(pBoard->getTurn()));

    for (auto q : mQuadrantRenderOrder) {
        renderQuadrant(q);
    }

    if (pBoard->getGamePhase() != GamePhase::PLAYING) renderGameEnd();

    mWindow.display();
}

void Game::setQuadrantToRenderLast(Quadrant q) {
    auto p = std::find(mQuadrantRenderOrder.begin(), mQuadrantRenderOrder.end(),q); // find
    std::swap(*p, mQuadrantRenderOrder[3]); // swap
}

void Game::renderQuadrant(const Quadrant q) {
    auto v = sf::Transform(mQuadrantTransforms[to_underlying(q)]).rotate(getRotationFor(q));
    mWindow.draw(mQuadrantShape, v);

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            auto c = pBoard->getColourAt(q, row, col);
            mCellShape.setFillColor(c ? toSFColor(*c) : sf::Color{150,0,0});
            mWindow.draw(mCellShape, v * mCellTransforms[row][col]);
        }
    }
}

void Game::renderGameEnd() {
    // opaque-ish overlay
    mWindow.draw(mGameEndOverlay);

    // set text
    static const std::array<const std::string, 3> array = { "Draw!", "White wins!", "Black wins!" };
    mGameEndText.setString(array[to_underlying(pBoard->getGamePhase()) - 1]);

    // centre text
    auto bounds = mGameEndText.getLocalBounds();
    mGameEndText.setOrigin({bounds.width / 2, bounds.height / 2});
    mGameEndText.setPosition(SCREEN_CENTRE);

    mWindow.draw(mGameEndText);
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

sf::Color Game::toSFColor(Colour c) {
    return (c == Colour::WHITE ? sf::Color::White : sf::Color::Black);
}