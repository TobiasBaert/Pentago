//
// Created by Tobias Baert on 18/08/2022.
//

#include "PlacementState.h"

#include "../Game.h"
#include "../Utilities.h"

PlacementState::PlacementState(Game& game) : IState(game) {}

void PlacementState::processEvent(sf::Event e) {
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

void PlacementState::processLMB() {
    if (Util::distanceLessOrEqualTo(pressEndPos, pressStartPos, 5.f)) {
        auto [q, c] = rGame.getCoordinateTupleFromPosition(pressEndPos);
        if (q && c) rGame.pBoard->placeAt(*q, c->first, c->second);
    }
}

