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
    return points[index];
}

void RoundedRectangle::calculatePoints() {
    points.clear();
    if (mPointsPerCorner == 1) {
        // it's a regular rectangle
        points = std::vector<sf::Vector2f>(std::begin(mOuterCorners), std::end(mOuterCorners));
    } else {
        const float interval = M_PI_2 / ((float) mPointsPerCorner - 1);

        for (size_t i = 0; i < mPointsPerCorner; i++) {
            float theta = interval * (float) i;
            float xOffset = mRadius * cos(theta);
            float yOffset = mRadius * sin(theta);

            points.emplace_back(mInnerCorners[0] + sf::Vector2f{-xOffset, -yOffset});
        }

        for (size_t i = 0; i < mPointsPerCorner; i++) {
            float theta = interval * (float) i;
            float xOffset = mRadius * cos(theta);
            float yOffset = mRadius * sin(theta);

            points.emplace_back(mInnerCorners[1] + sf::Vector2f{ yOffset, -xOffset});
        }

        for (size_t i = 0; i < mPointsPerCorner; i++) {
            float theta = interval * (float) i;
            float xOffset = mRadius * cos(theta);
            float yOffset = mRadius * sin(theta);

            points.emplace_back(mInnerCorners[2] + sf::Vector2f{ xOffset, yOffset});
        }

        for (size_t i = 0; i < mPointsPerCorner; i++) {
            float theta = interval * (float) i;
            float xOffset = mRadius * cos(theta);
            float yOffset = mRadius * sin(theta);

            points.emplace_back(mInnerCorners[3] + sf::Vector2f{-yOffset, xOffset});
        }

    }
    points.shrink_to_fit();
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

