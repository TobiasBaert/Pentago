//
// Created by Tobias Baert on 18/08/2022.
//

#include "PlacementState.h"

#include "../Game.h"
#include "../Utilities.h"

PlacementState::PlacementState(Game& game) : IState(game) {}

void PlacementState::processInputs() {
    switch (mLMBState) {
        case LMBState::IDLE:
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                mLMBState = LMBState::HELD;
                pressStartPos = getMousePosition();
            }
            break;
        case LMBState::HELD:
            if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                mLMBState = LMBState::IDLE;
                processLMB(getMousePosition());
            }
            break;
    }
}

void PlacementState::processLMB(sf::Vector2f pressEndPos) {
    if (Util::distanceLessOrEqualTo(pressStartPos, pressEndPos, 15.f)) {
        auto [q, c] = rGame.getCoordinateTupleFromPosition(pressEndPos);
        if (q && c) rGame.pBoard->placeAt(*q, c->first, c->second);
    }
}