//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_BOARD_H_DATE_27_07_2022_TIME_14_01
#define PENTAGO_BOARD_H_DATE_27_07_2022_TIME_14_01

#include <optional>
#include <type_traits>

enum class Colour : bool {
    WHITE = true,
    BLACK = false,
};

enum class RotationDir {
    CLOCKWISE,
    COUNTERCLOCKWISE
};

enum class Quadrant : int {
    NORTHWEST = 0,
    NORTHEAST,
    SOUTHWEST,
    SOUTHEAST
};

template<typename E> constexpr auto to_underlying(E e) -> typename std::underlying_type<E>::type  {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

class IBoard {
public:
    typedef std::optional<Colour> OptionalColour;

    virtual ~IBoard() = default;

    [[nodiscard]] virtual Colour getTurn() const = 0;

    [[nodiscard]] virtual OptionalColour colourAt(Quadrant q, int x, int y) const;
    [[nodiscard]] virtual OptionalColour colourAt(int x, int y) const = 0;

    virtual void placeAt(Colour col, int x, int y) = 0;
    virtual void rotate(Quadrant q, RotationDir d) = 0;
    virtual void advanceTurn() = 0;

    [[nodiscard]] virtual bool isWon() const;
    [[nodiscard]] virtual OptionalColour getWinner() const = 0;
};


#endif //PENTAGO_BOARD_H_DATE_27_07_2022_TIME_14_01
