//
// Created by Tobias Baert on 27/07/2022.
//

#include "IBoard.h"

bool IBoard::isBlackToPlay() const {
    return getTurn() == Colour::BLACK;
}

IBoard::OptionalColour IBoard::colourAt(Quadrant q, int x, int y) const {
    x += 3 * (to_underlying(q) / 2);
    y += 3 * (to_underlying(q) % 2);
    return colourAt(x,y);
}

bool IBoard::isWon() const {
    return getWinner().has_value();
}