//
// Created by Tobias Baert on 27/07/2022.
//

#include "RoundedRectangle.h"

#define _USE_MATH_DEFINES // for pi as M_PI
#include <cmath>

RoundedRectangle::RoundedRectangle(const sf::Vector2f& size, const float radius, size_t pointsPerCorner)
        : mSize(size)
        , mOuterCorners{ sf::Vector2f{0,0},
                         sf::Vector2f{mSize.x, 0},
                         sf::Vector2f{mSize.x, mSize.y},
                         sf::Vector2f{0, mSize.y}}
        , mInnerCorners{mOuterCorners[0] + sf::Vector2f{ radius,  radius},
                        mOuterCorners[1] + sf::Vector2f{-radius,  radius},
                        mOuterCorners[2] + sf::Vector2f{-radius, -radius},
                        mOuterCorners[3] + sf::Vector2f{ radius, -radius}}
        , mRadius(radius)
        , mPointsPerCorner(pointsPerCorner) {
    calculatePoints();
    update();
}

size_t RoundedRectangle::getPointCount() const {
    return 4*mPointsPerCorner;
}

sf::Vector2f RoundedRectangle::getPoint(std::size_t index) const {
    return mPoints[index / mPointsPerCorner][index % mPointsPerCorner];
}

void RoundedRectangle::calculatePoints() {
    // clear previous points, don't want to try and reuse them
    for (auto& e : mPoints) e.clear();

    if (mPointsPerCorner == 1) { // it's a regular rectangle...
        // ... so copy the outer corners
        for(int i = 0; i < 4; i++) mPoints[i].emplace_back(mOuterCorners[i]);
    } else {
        const float interval = M_PI_2 / ((float) mPointsPerCorner - 1); // diff in angle between samples of circle

        for (size_t i = 0; i < mPointsPerCorner; i++) {
            float theta = interval * (float) i;
            float oneToZero = mRadius * cos(theta); // value goes from 1 -> 0 as theta increases
            float zeroToOne = mRadius * sin(theta); // value goes from 0 -> 1 as theta increases

            // best to sketch on piece of paper
            mPoints[0].emplace_back(mInnerCorners[0] + sf::Vector2f{-oneToZero, -zeroToOne});
            mPoints[1].emplace_back(mInnerCorners[1] + sf::Vector2f{ zeroToOne, -oneToZero});
            mPoints[2].emplace_back(mInnerCorners[2] + sf::Vector2f{ oneToZero,  zeroToOne});
            mPoints[3].emplace_back(mInnerCorners[3] + sf::Vector2f{-zeroToOne,  oneToZero});
        }
    }
}

const sf::Vector2f& RoundedRectangle::getSize() const {
    return mSize;
}

void RoundedRectangle::setSize(const sf::Vector2f& size) {
    RoundedRectangle::mSize = size;
    calculatePoints();
    update();
}

float RoundedRectangle::getRadius() const {
    return mRadius;
}

void RoundedRectangle::setRadius(float radius) {
    RoundedRectangle::mRadius = radius;
    calculatePoints();
    update();
}

size_t RoundedRectangle::getPointsPerCorner() const {
    return mPointsPerCorner;
}

void RoundedRectangle::setPointsPerCorner(size_t pointsPerCorner) {
    RoundedRectangle::mPointsPerCorner = pointsPerCorner;
    calculatePoints();
    update();
}

