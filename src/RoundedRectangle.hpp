//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_ROUNDEDRECTANGLE_H_DATE_27_07_2022_TIME_21_33
#define PENTAGO_ROUNDEDRECTANGLE_H_DATE_27_07_2022_TIME_21_33

#define _USE_MATH_DEFINES // for pi as M_PI

#include <array>
#include <cmath>
#include <SFML/Graphics/Shape.hpp>

template<size_t pointsPerCorner = 6> class RoundedRectangle : public sf::Shape {
public:

    RoundedRectangle(const sf::Vector2f& size, float radius);

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
RoundedRectangle<pointsPerCorner>::RoundedRectangle(const sf::Vector2f& size, float radius)
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
size_t RoundedRectangle<pointsPerCorner>::getPointCount() const {
    return 4 * pointsPerCorner;
}

template<size_t pointsPerCorner>
sf::Vector2f RoundedRectangle<pointsPerCorner>::getPoint(std::size_t index) const {
    return mPoints[index / pointsPerCorner][index % pointsPerCorner];
}

template<>
const sf::Vector2f& RoundedRectangle<>::getSize() const {
    return mSize;
}

template<>
void RoundedRectangle<>::setSize(const sf::Vector2f& size) {
    mSize = size;
    calculatePoints();
    update();
}

template<>
float RoundedRectangle<>::getRadius() const {
    return mRadius;
}

template<>
void RoundedRectangle<>::setRadius(float radius) {
    mRadius = radius;
    calculatePoints();
    update();
}

template<>
void RoundedRectangle<1>::calculatePoints() {
    for (int i = 0; i < 4; i++) mPoints[i][0] = mOuterCorners[i];
}

template<size_t pointsPerCorner>
void RoundedRectangle<pointsPerCorner>::calculatePoints() {
    // the points of each corner are obtained by sampling a quarter-circle at pointsPerCorner equidistant points.
    // interval indicates the difference in angle between subsequent samples.
    static constexpr float interval = M_PI_2 / ((float) pointsPerCorner - 1);
    static constexpr std::array<float, pointsPerCorner> thetas;
    static constexpr std::array<float, pointsPerCorner> cosines;
    static constexpr std::array<float, pointsPerCorner> sines;

    for (size_t i = 0; i < pointsPerCorner; i++) {
        thetas[i] = interval * (float) i;
        cosines[i] = cos(thetas[i]);
        sines[i] = sin(thetas[i]);
    }

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
