//
// Created by Tobias Baert on 27/07/2022.
//

#include "IBoard.h"

bool IBoard::isEmpty(int x, int y) const {
    return !colourAt(x,y).has_value();
}

bool IBoard::isWhite(int x, int y) const {
    return to_underlying(*colourAt(x,y));
}

bool IBoard::isBlack(int x, int y) const {
    return !to_underlying(*colourAt(x,y));
}

bool IBoard::isWon() const {
    return getWinner().has_value();
}

bool IBoard::isWhiteToPlay() const {
    return getTurn() == Colour::WHITE;
}

bool IBoard::isBlackToPlay() const {
    return getTurn() == Colour::BLACK;
}
