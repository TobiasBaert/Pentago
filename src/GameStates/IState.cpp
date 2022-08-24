//
// Created by Tobias Baert on 19/08/2022.
//

#include "IState.h"

#include "../Game.h"

sf::Vector2f IState::toVec2f(sf::Vector2i p) {
    return {static_cast<float>(p.x), static_cast<float>(p.y)};
}

sf::Vector2f IState::getMousePosition() const {
    return toVec2f(sf::Mouse::getPosition(rGame.mWindow));
}

