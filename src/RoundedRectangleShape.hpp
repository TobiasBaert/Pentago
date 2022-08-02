//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_ROUNDEDRECTANGLE_H_DATE_27_07_2022_TIME_21_33
#define PENTAGO_ROUNDEDRECTANGLE_H_DATE_27_07_2022_TIME_21_33

#define _USE_MATH_DEFINES // for pi as M_PI

#include <array>
#include <cmath>
#include <SFML/Graphics/Shape.hpp>

#include "compile_time/math.hpp"

template<size_t pointsPerCorner> class RoundedRectangleShape : public sf::Shape {
public:

    RoundedRectangleShape(const sf::Vector2f& size, float radius);

    sf::Vector2f getSize() const;
    void setSize(const sf::Vector2f& mSize);

    float getRadius() const;
    void setRadius(float mRadius);

    size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t index) const override;

    bool withinLocalBounds(sf::Vector2f localPos);

private:
    float mRadius;
    sf::FloatRect mOuterRect;

    sf::FloatRect mInnerRect;
    sf::Vector2f mInnerCorners[4];  // focus points of the corners
    sf::Vector2f mPoints[4][pointsPerCorner]; // points of each corner, from upper left corner and proceeding clockwise

    void updateInternals();
};

template<size_t pointsPerCorner>
RoundedRectangleShape<pointsPerCorner>::RoundedRectangleShape(const sf::Vector2f& size, float radius)
        : mRadius(radius)
        , mOuterRect(0, 0, size.x, size.y) {
    updateInternals();
    update();
}

template<size_t pointsPerCorner>
sf::Vector2f RoundedRectangleShape<pointsPerCorner>::getSize() const {
    return {mOuterRect.width, mOuterRect.height};
}

template<size_t pointsPerCorner>
void RoundedRectangleShape<pointsPerCorner>::setSize(const sf::Vector2f& size) {
    mOuterRect.width = size.x;
    mOuterRect.height = size.y;
    updateInternals();
    update();
}

template<size_t pointsPerCorner>
float RoundedRectangleShape<pointsPerCorner>::getRadius() const {
    return mRadius;
}

template<size_t pointsPerCorner>
void RoundedRectangleShape<pointsPerCorner>::setRadius(float radius) {
    mRadius = radius;
    updateInternals();
    update();
}

template<size_t pointsPerCorner>
size_t RoundedRectangleShape<pointsPerCorner>::getPointCount() const {
    return 4 * pointsPerCorner;
}

template<size_t pointsPerCorner>
sf::Vector2f RoundedRectangleShape<pointsPerCorner>::getPoint(std::size_t index) const {
    return mPoints[index / pointsPerCorner][index % pointsPerCorner];
}

template<size_t pointsPerCorner>
constexpr auto initThetas() {
    constexpr float interval = M_PI_2 / ((float) pointsPerCorner - 1);
    return [] {
        std::array<float, pointsPerCorner> tmp{};
        for (size_t i = 0; i < pointsPerCorner; i++) tmp[i] = interval * (float) i;
        return tmp;
    }();
}

template<size_t pointsPerCorner>
constexpr auto initCosines(const std::array<float, pointsPerCorner>& thetas) {
    std::array<float, pointsPerCorner> tmp{};
    auto cosMap = [] (auto e) {return compile_time::cos(e);};
    std::transform(thetas.cbegin(), thetas.cend(), tmp.begin(), cosMap);
    return tmp;
}

template<size_t pointsPerCorner>
constexpr auto initSines(const std::array<float, pointsPerCorner>& thetas) {
    std::array<float, pointsPerCorner> tmp{};
    auto sinMap = [] (auto e) {return compile_time::sin(e);};
    std::transform(thetas.cbegin(), thetas.cend(), tmp.begin(), sinMap);
    return tmp;
}

template<size_t pointsPerCorner>
bool RoundedRectangleShape<pointsPerCorner>::withinLocalBounds(sf::Vector2f localPos) {
    if (!mOuterRect.contains(localPos)) return false;
    if (mInnerRect.contains(localPos)) return true;

    // produce a new position by clamping localPos to the inner rectangle
    float clampedX = std::min(std::max(localPos.x, mInnerCorners[0].x), mInnerCorners[2].x);
    float clampedY = std::min(std::max(localPos.y, mInnerCorners[0].y), mInnerCorners[2].y);

    // calculate distance from clamped point to localPos
    double squaredDistance = std::pow(clampedX - localPos.x, 2) + std::pow(clampedY - localPos.y, 2);
    return squaredDistance <= std::pow(mRadius, 2);
}

template<size_t pointsPerCorner>
void RoundedRectangleShape<pointsPerCorner>::updateInternals() {
    // set inner rectangle
    mInnerRect.left     = mRadius;
    mInnerRect.top      = mRadius;
    mInnerRect.width    = mOuterRect.width - 2 * mRadius;
    mInnerRect.height   = mOuterRect.height - 2 * mRadius;

    // set inner points
    mInnerCorners[0] = {mInnerRect.left                     , mInnerRect.top};
    mInnerCorners[1] = {mInnerRect.left + mInnerRect.width  , mInnerRect.top};
    mInnerCorners[2] = {mInnerRect.left + mInnerRect.width  , mInnerRect.top + mInnerRect.height};
    mInnerCorners[3] = {mInnerRect.left                     , mInnerRect.top + mInnerRect.height};

    // calculate outline points
    // the points of each corner are obtained by sampling a quarter-circle at pointsPerCorner equidistant points.
    // interval indicates the difference in angle between subsequent samples.
    static constexpr std::array<float, pointsPerCorner> thetas = initThetas<pointsPerCorner>();
    static constexpr std::array<float, pointsPerCorner> cosines = initCosines(thetas);
    static constexpr std::array<float, pointsPerCorner> sines = initSines(thetas);

    for (size_t i = 0; i < pointsPerCorner; i++) {
        float oneToZero = mRadius * cosines[i]; // value goes from 1 -> 0 as theta increases
        float zeroToOne = mRadius * sines[i]; // value goes from 0 -> 1 as theta increases

        // best to sketch on piece of paper
        mPoints[0][i] = mInnerCorners[0] + sf::Vector2f{-oneToZero, -zeroToOne};
        mPoints[1][i] = mInnerCorners[1] + sf::Vector2f{ zeroToOne, -oneToZero};
        mPoints[2][i] = mInnerCorners[2] + sf::Vector2f{ oneToZero,  zeroToOne};
        mPoints[3][i] = mInnerCorners[3] + sf::Vector2f{-zeroToOne,  oneToZero};
    }

}


#endif //PENTAGO_ROUNDEDRECTANGLE_H_DATE_27_07_2022_TIME_21_33
