//
// Created by Tobias Baert on 18/08/2022.
//

#include "RotationState.h"

#include "Game.h"
#include "Utilities.h"

RotationState::RotationState(Game& game) : IState(game) {}

void RotationState::processEvent(sf::Event e) {
    switch (e.type) {
        case sf::Event::MouseButtonPressed:
            if (e.mouseButton.button == sf::Mouse::Button::Left) {
                pressStartPos = toVec2f(sf::Mouse::getPosition(rGame.mWindow));
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (e.mouseButton.button == sf::Mouse::Button::Left) {
                pressEndPos = toVec2f(sf::Mouse::getPosition(rGame.mWindow));
                processLMB();
            }
            break;
        default:
            break;
    }
}

void RotationState::processLMB() {
    sf::Vector2f v;
    auto q = rGame.getQuadrantFromPosition(pressEndPos, v);
    if (q) rGame.pBoard->rotate(*q, RotationDir::COUNTERCLOCKWISE);
}

