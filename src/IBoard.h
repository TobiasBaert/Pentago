//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_BOARD_H_DATE_27_07_2022_TIME_14_01
#define PENTAGO_BOARD_H_DATE_27_07_2022_TIME_14_01

#include <optional>

enum Colour : bool {
    WHITE = true,
    BLACK = false,
};

enum RotationDir : int {
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = -1
};

enum class Quadrant : int {
    NORTHWEST = 0,
    NORTHEAST,
    SOUTHWEST,
    SOUTHEAST
};

class IBoard {
public:
    typedef std::optional<Colour> OptionalColour;

    virtual bool whiteToPlay();
    virtual bool blackToPlay();
    virtual Colour getTurn() = 0;

    virtual bool isEmpty(int x, int y);
    virtual bool isWhite(int x, int y);
    virtual bool isBlack(int x, int y);
    virtual OptionalColour colourAt(int x, int y) = 0;

    virtual void placeAt(Colour col, int x, int y) = 0;
    virtual void rotate(Quadrant q, RotationDir d) = 0;
    virtual void advanceTurn() = 0;

    virtual bool isWon();
    virtual OptionalColour getWinner() = 0;
};


#endif //PENTAGO_BOARD_H_DATE_27_07_2022_TIME_14_01
