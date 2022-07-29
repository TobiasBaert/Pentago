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

template<size_t pointsPerCorner = 6> class RoundedRectangleShape : public sf::Shape {
public:

    RoundedRectangleShape(const sf::Vector2f& size, float radius);

    const sf::Vector2f& getSize() const;
    void setSize(const sf::Vector2f& mSize);

    float getRadius() const;
    void setRadius(float mRadius);

    size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t index) const override;

private:
    sf::Vector2f mSize;             // dimensions of bounding box
    sf::Vector2f mOuterCorners[4];  // corners of bounding box
    sf::Vector2f mInnerCorners[4];  // focus points of the corners
    sf::Vector2f mPoints[4][pointsPerCorner]; // points of each corner, from upper left corner and proceeding clockwise
    float mRadius{};

    void calculatePoints();
};

template<size_t pointsPerCorner>
RoundedRectangleShape<pointsPerCorner>::RoundedRectangleShape(const sf::Vector2f& size, float radius)
        : mSize(size)
        , mOuterCorners{ sf::Vector2f{0,0},
                         sf::Vector2f{mSize.x, 0},
                         sf::Vector2f{mSize.x, mSize.y},
                         sf::Vector2f{0, mSize.y}}
        , mInnerCorners{mOuterCorners[0] + sf::Vector2f{ radius,  radius},
                        mOuterCorners[1] + sf::Vector2f{-radius,  radius},
                        mOuterCorners[2] + sf::Vector2f{-radius, -radius},
                        mOuterCorners[3] + sf::Vector2f{ radius, -radius}}
        , mRadius(radius) {
    calculatePoints();
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

template<>
const sf::Vector2f& RoundedRectangleShape<>::getSize() const {
    return mSize;
}

template<>
void RoundedRectangleShape<>::setSize(const sf::Vector2f& size) {
    mSize = size;
    calculatePoints();
    update();
}

template<>
float RoundedRectangleShape<>::getRadius() const {
    return mRadius;
}

template<>
void RoundedRectangleShape<>::setRadius(float radius) {
    mRadius = radius;
    calculatePoints();
    update();
}

template<>
void RoundedRectangleShape<1>::calculatePoints() {
    for (int i = 0; i < 4; i++) mPoints[i][0] = mOuterCorners[i];
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
void RoundedRectangleShape<pointsPerCorner>::calculatePoints() {
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
