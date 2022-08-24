//
// Created by Tobias Baert on 01/08/2022.
//

#ifndef PENTAGO_UTILITIES_H_DATE_01_08_2022_TIME_23_22
#define PENTAGO_UTILITIES_H_DATE_01_08_2022_TIME_23_22

#include <array>
#include <cstddef>
#include <cmath>

namespace Util {

    /**
     * Converts an angle from radians to degrees.
     * @param radians the angle in radians
     * @return the same angle in degrees, with the same sign as the input.
     */
    inline double toDegrees(double radians) {
        return radians * 180 / M_PI;
    }

    /**
     * Converts an angle from degrees to radians.
     * @param degrees the angle in degrees
     * @return the same angle in radians, with the same sign as the input.
     */
    inline double toRadians(double degrees) {
        return degrees * M_PI / 180;
    }

    /**
     * Calculates the square of the distance between two points.
     * @param first first point
     * @param second second point
     * @return the square of the euclidean distance between the first and second point.
     */
    template<typename T>
    inline double squaredDistance(sf::Vector2<T> first, sf::Vector2<T> second) {
        return std::pow(first.x - second.x, 2) + std::pow(first.y - second.y, 2);
    }

    /**
     * Checks if the distance between two points is less than or equal to a given amount.
     * @param first first point
     * @param second second point
     * @param maxDistance maximum distance
     * @return true iff the distance between the two points <= the maximum distance
     */
    template<typename T>
    inline bool distanceLessOrEqualTo(sf::Vector2<T> first, sf::Vector2<T> second, float maxDistance) {
        return squaredDistance(first, second) <= std::pow(maxDistance, 2);
    }

    /*
     * Necessary to fill std::array for types without a default constructor.
     * Source: https://stackoverflow.com/questions/57756557/initializing-a-stdarray-with-a-constant-value
     */
    template <typename T, std::size_t ... Is>
    constexpr std::array<T, sizeof...(Is)>
    create_array(T value, std::index_sequence<Is...>) {
        return {{(Is, value)...}};
    }

    /**
     * Shorthand for compile-time, uniform initialisation of an array.
     * @tparam N the length of the array
     * @tparam T the type of values in the array
     * @param value the value with which to fill the entire array
     * @return returns an array of length N, filled entirely with the given value
     */
    template <std::size_t N, typename T>
    constexpr std::array<T, N> create_array(const T& value) {
        return Util::create_array(value, std::make_index_sequence<N>());
    }
}

#endif //PENTAGO_UTILITIES_H_DATE_01_08_2022_TIME_23_22
