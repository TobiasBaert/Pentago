//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_BOARD_H_DATE_27_07_2022_TIME_14_01
#define PENTAGO_BOARD_H_DATE_27_07_2022_TIME_14_01

#include <optional>
#include <type_traits>

/**
 * The two playing colours.
 */
enum class Colour : bool {
    WHITE = true,
    BLACK = false,
};

/**
 * Inverts from one colour to the other.
 * @param c the colour to invert
 * @return if c is WHITE, return BLACK, otherwise return WHITE
 */
inline Colour operator!(Colour c) {
    return (c == Colour::WHITE ? Colour::BLACK : Colour::WHITE);
}

/**
 * The two directions to rotate quadrants in.
 */
enum class RotationDir : int { // FIXME to size_t
    CLOCKWISE = 0,
    COUNTERCLOCKWISE
};

/**
 * The four names for the four 3x3 quadrants.
 */
enum class Quadrant : int { // FIXME to size_t
    NORTHWEST = 0,
    NORTHEAST,
    SOUTHWEST,
    SOUTHEAST
};

/**
 * Converts an enum value to the corresponding value of the underlying type.
 * @tparam E the enum type get the underlying type from
 * @param e the enum value to convert to the underlying type
 * @return the enum value converted to the corresponding value of the underlying type
 */
template<typename E>
constexpr auto to_underlying(E e) noexcept {
    return static_cast<typename std::underlying_type_t<E>>(e);
}

class IBoard {
public:
    typedef std::optional<Colour> OptionalColour;

    virtual ~IBoard() = default;

    /**
     * Return the board to its initial state.
     */
    virtual void reset() = 0;

    /**
     * Return the colour currently playing.
     * @return the colour that is currently playing
     */
    [[nodiscard]] virtual Colour getTurn() const = 0;

    /**
     * Return the colour of the marble in the cell at the given row and column, if any.
     * @param x the global row, in the range [0,5]
     * @param y the global column, in the range [0,5]
     * @return if a colour is present in the cell at given row and column, otherwise return an empty optional
     */
    [[nodiscard]] virtual OptionalColour getColourAt(int x, int y) const = 0;

    /**
     * Return the colour of the marble in the cell at the given row and column WITHIN a particular quadrant, if any.
     * @param q the quadrant
     * @param x the row within the quadrant, in the range [0,2]
     * @param y the column within the quadrant, in the range [0,2]
     * @return if a colour is present in the cell at given row and column within the chosen quadrant, otherwise
     * return an empty optional
     */
    [[nodiscard]] virtual OptionalColour getColourAt(Quadrant q, int x, int y) const;

    /**
     * Place a marble of the currently playing colour in the cell at the given row and column.
     * @param x the global row, in the range [0,5]
     * @param y the global column, in the range [0,5]
     */
    virtual void placeAt(int x, int y) = 0;

    /**
     * Place a marble of the currently playing colour in the cell at the given row and column WITHIN a particular
     * quadrant.
     * @param q the quadrant
     * @param x the row within the quadrant, in the range [0,2]
     * @param y the column within the quadrant, in the range [0,2]
     */
    virtual void placeAt(Quadrant q, int x, int y );

    /**
     * Rotate a given quadrant 90 degrees in the given direction.
     * @param q the quadrant to rotate
     * @param d the direction to rotate in
     */
    virtual void rotate(Quadrant q, RotationDir d) = 0;

    /**
     * Switches the turn to the other colour.
     */
    virtual void advanceTurn() = 0;

    /**
     * Checks if the game has reached an end condition.
     * @return true iff any colour has placed 5 marbles in a row on the board or the board is completely occupied.
     */
    [[nodiscard]] virtual bool hasEnded() const = 0;

    /**
     * Returns the victor of a game, if any.
     *
     * If an empty optional is returned, then interpretation depends on hasEnded(): if hasEnded(), then the result
     * represents a draw. Otherwise, the result indicates the game has not yet ended.
     * @return if both players have placed 5 marbles in a row, then return an empty optional. If just one player
     * meets the condition, then return that player's colour. Otherwise, return an empty optional.
     */
    [[nodiscard]] virtual OptionalColour getWinner() const = 0;
};


#endif //PENTAGO_BOARD_H_DATE_27_07_2022_TIME_14_01
