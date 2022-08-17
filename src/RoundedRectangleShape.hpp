//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_ROUNDEDRECTANGLE_H_DATE_27_07_2022_TIME_21_33
#define PENTAGO_ROUNDEDRECTANGLE_H_DATE_27_07_2022_TIME_21_33

#define _USE_MATH_DEFINES // for pi / 2 as M_PI_2

#include <array>
#include <cmath>
#include <SFML/Graphics/Shape.hpp>

#include "compile_time/math.hpp"

/**
 * This class defines a rectangle shape with rounded corners of a certain radius. The rounded corners are approximated
 * by a fixed number of equidistant points that lie on a quarter-circle of the specified radius. The number of
 * points per corner determines the smoothness of the curve. The radius determines how far the corners start and end.
 *
 * The radius is a signed float without further restrictions. However, it should be a positive value and the class is
 * designed around "sensible" values, i.e., a radius that does not exceed half the size in either dimension.
 *
 * Some choices for the number of points per corner (PPC) and the radius cause the shape to degenerate to a simpler
 * shape. For example, choosing PPC = 1 results in a regular rectangle. In that case, one should use the available
 * sf::Rectangle class instead.
 *
 * This class internally uses an "inner" and "outer" rectangle. The outer rectangle is essentially the bounding
 * rectangle, the rectangle with the same width and height but no rounded corners. Its upper left corner is thus at
 * (0,0) in the local coordinate system. The inner rectangle is obtained by connecting the centre of the
 * quarter-circle of each corner. Thus in each in dimension it is reduced by twice the radius and the upper-left
 * corner is at (radius, radius) in the local coordinate system.
 *
 * By inheriting from sf::Shape, the class inherits both sf::Drawable and, importantly, sf::Transformable.
 * @tparam pointsPerCorner the number of points per corner, a non-zero value.
 */
template<size_t pointsPerCorner> class RoundedRectangleShape : public sf::Shape {
public:

    /**
     * Creates a rounded rectangle shape with given size and radius.
     * @param size the width and height of the outer rectangle
     * @param radius the radius of the corners
     */
    RoundedRectangleShape(const sf::Vector2f& size, float radius);

    /**
     * Returns the dimensions of the bounding box of the shape.
     * @return the width and height of the shape
     */
    sf::Vector2f getSize() const;

    /**
     * Sets the dimensions of the bounding box of the shape.
     * @param mSize the desired width and height
     */
    void setSize(const sf::Vector2f& mSize);

    /**
     * Returns the radius of the corners.
     * @return the radius of the corners
     */
    float getRadius() const;

    /**
     * Sets the radius for the corners.
     * @param mRadius the desired radius
     */
    void setRadius(float mRadius);

    size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t index) const override;

    /**
     * Checks if a given position lies within the shape, taking into account the transforms applied to the shape.
     *
     * @param globalPos the position
     * @return true iff applying the inverse transformation to the given position results in a position within the
     * local bounds of the shape.
     */
    bool withinGlobalBounds(sf::Vector2f globalPos) const;

    /**
     * Checks if a given position lies within the shape, without taking into account any transforms.
     * @param localPos the position
     * @return true iff the position lies within the inner rectangle or the distance between the position and the
     * closest point of the inner rectangle is no more than the radius.
     */
    bool withinLocalBounds(sf::Vector2f localPos) const;

private:
    float mRadius;

    sf::FloatRect mOuterRect;
    sf::FloatRect mInnerRect;

    /**
     * The corners of the inner rectangle, derived and stored for easy access.
     */
    sf::Vector2f mInnerCorners[4];

    /**
     * The stored points that make up the shape.
     *
     * The first index indicates which corner the point belongs to (upper-left corner first, then continuing
     * clockwise) and the second index which point of the corner (in clockwise order for each corner).
     */
    sf::Vector2f mPoints[4][pointsPerCorner];

    /**
     * Synchronise the points of the outline with a change in the size or radius.
     */
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

/**
 * The following 3 functions serve to calculate the required offsets (vertical and horizontal) for the corner points at
 * compile-time.
 *
 * initThetas() calculates the series of radians where the quarter-circle will be sampled.
 * initCosines() maps the series to their cosines to get a series that goes from 1 to 0, along the sampled points.
 * initSines() maps the series to their sines to get a series that goes from 0 to 1, along the sampled points.
 *
 * Every corner can reuse these same results, although their interpretation and the applied sign might change.
 *
 * FIXME extract from global namespace
 */


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
bool RoundedRectangleShape<pointsPerCorner>::withinGlobalBounds(sf::Vector2f globalPos) const {
    return withinLocalBounds(getInverseTransform().transformPoint(globalPos));
}

template<size_t pointsPerCorner>
bool RoundedRectangleShape<pointsPerCorner>::withinLocalBounds(sf::Vector2f localPos) const {
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
