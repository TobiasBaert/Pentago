//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_ROUNDEDRECTANGLE_H_DATE_27_07_2022_TIME_21_33
#define PENTAGO_ROUNDEDRECTANGLE_H_DATE_27_07_2022_TIME_21_33


#include <SFML/Graphics/Shape.hpp>

class RoundedRectangle : public sf::Shape {
public:
    explicit RoundedRectangle(
            const sf::Vector2f& size,
            float radius,
            size_t pointsPerCorner = 5);

    const sf::Vector2f& getSize() const;

    void setSize(const sf::Vector2f& mSize);

    float getRadius() const;

    void setRadius(float mRadius);

    size_t getPointsPerCorner() const;

    void setPointsPerCorner(size_t mPointsPerCorner);

    size_t getPointCount() const override;

    sf::Vector2f getPoint(std::size_t index) const override;

private:
    sf::Vector2f mSize;             // dimensions of bounding box
    sf::Vector2f mOuterCorners[4];  // corners of bounding box
    sf::Vector2f mInnerCorners[4];  // focus points of the corners
    std::vector<sf::Vector2f> mPoints[4]; // points of each corner, from upper left corner and proceeding clockwise
    float mRadius;
    size_t mPointsPerCorner;

    void calculatePoints();
};


#endif //PENTAGO_ROUNDEDRECTANGLE_H_DATE_27_07_2022_TIME_21_33
