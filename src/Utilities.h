//
// Created by Tobias Baert on 01/08/2022.
//

#ifndef PENTAGO_UTILITIES_H_DATE_01_08_2022_TIME_23_22
#define PENTAGO_UTILITIES_H_DATE_01_08_2022_TIME_23_22

#include <array>
#include <cstddef>
#include <cmath>

namespace Util {

    inline double squaredDistance(sf::Vector2f first, sf::Vector2f second) {
        return std::pow(first.x - second.x, 2) + std::pow(first.y - second.y, 2);
    }

    inline bool distanceLessThan(sf::Vector2f first, sf::Vector2f second, float maxDistance) {
        return squaredDistance(first, second) <= std::pow(maxDistance, 2);
    }

    /*
     * Necessary to fill std::array for types without a default constructor.
     * Source: https://stackoverflow.com/questions/57756557/initializing-a-stdarray-with-a-constant-value
     */
    template <typename T, std::size_t ... Is>
    constexpr std::array<T, sizeof...(Is)>
    create_array(T value, std::index_sequence<Is...>) {
        // cast Is to void to remove the warning: unused value
        return {{(static_cast<void>(Is), value)...}};
    }

    template <std::size_t N, typename T>
    constexpr std::array<T, N> create_array(const T& value) {
        return Util::create_array(value, std::make_index_sequence<N>());
    }
}

#endif //PENTAGO_UTILITIES_H_DATE_01_08_2022_TIME_23_22
