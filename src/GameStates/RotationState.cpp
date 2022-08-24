//
// Created by Tobias Baert on 18/08/2022.
//

#include "RotationState.h"

#include "../Game.h"
#include "IBoard.h"
#include <cmath>
#include <iostream>

RotationState::RotationState(Game& game) : IState(game) {}

void RotationState::processInputs() {
    switch (mLMBState) {
        case LMBState::IDLE:
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                mLMBState = LMBState::HELD;
                sf::Vector2f startPosRelToQuadrant;
                auto q = rGame.getQuadrantFromPosition(getMousePosition(), startPosRelToQuadrant);
                if (q) {
                    mStartAndEnd = std::make_pair(startPosRelToQuadrant, startPosRelToQuadrant);
                    setRotatingQuadrant(*q);
                }
            }
            break;
            
        case LMBState::HELD:
            if (mStartAndEnd) {
                mStartAndEnd->second = getRelativeMousePosition();
                syncRotation();
            }

            if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                mLMBState = LMBState::IDLE;
                doRotation();
            }
            break;
    }
}

sf::Vector2f RotationState::getRelativeMousePosition() {
    assert(rGame.mRotatingQuadrant);
    return rGame.mQuadrantTransforms[to_underlying(rGame.mRotatingQuadrant->first)]
                .getInverse()
                .transformPoint(getMousePosition());
}

float RotationState::getAngle() {
    assert(mStartAndEnd);
    auto [s,e] = *mStartAndEnd;
    float dot = s.x * e.x + s.y * e.y;
    float det = s.x * e.y - s.y * e.x;
    return atan2(det, dot);
}

void RotationState::setRotatingQuadrant(Enums::Quadrant q) {
    rGame.mRotatingQuadrant.emplace(q, 0.f);
    rGame.setQuadrantToRenderLast(q);
}

void RotationState::syncRotation() {
    auto rawAngle = static_cast<float>(Util::toDegrees(getAngle()));
    rGame.mRotatingQuadrant->second = rawAngle;
}

void RotationState::doRotation() {
    assert(rGame.mRotatingQuadrant);
    auto [q, angle] = *(rGame.mRotatingQuadrant);
    if (75.f <= abs(angle) && abs(angle) <= 105.f) {
        if (angle > 0) rGame.pBoard->rotate(q, Enums::RotationDir::CLOCKWISE);
        else rGame.pBoard->rotate(q, Enums::RotationDir::COUNTERCLOCKWISE);
    }
    mStartAndEnd.reset();
    rGame.mRotatingQuadrant.reset();
}


