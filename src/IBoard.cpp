//
// Created by Tobias Baert on 27/07/2022.
//

#include "IBoard.h"

bool IBoard::isWhiteToPlay() const {
    return getTurn() == Colour::WHITE;
}

bool IBoard::isBlackToPlay() const {
    return getTurn() == Colour::BLACK;
}

bool IBoard::isEmpty(int x, int y) const {
    return !colourAt(x,y);
}

bool IBoard::isWhite(int x, int y) const {
    return !isEmpty(x,y) && to_underlying(*colourAt(x,y));
}

bool IBoard::isBlack(int x, int y) const {
    return !isEmpty(x,y) && !to_underlying(*colourAt(x,y));
}

IBoard::OptionalColour IBoard::colourAt(Quadrant q, int x, int y) const {
    x += 3 * (to_underlying(q) / 2);
    y += 3 * (to_underlying(q) % 2);
    return colourAt(x,y);
}

bool IBoard::isWon() const {
    return getWinner().has_value();
}