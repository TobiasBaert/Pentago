//
// Created by Tobias Baert on 27/07/2022.
//

#include "IBoard.h"

IBoard::OptionalColour IBoard::getColourAt(Quadrant q, int x, int y) const {
    x += 3 * (to_underlying(q) / 2);
    y += 3 * (to_underlying(q) % 2);
    return getColourAt(x, y);
}

void IBoard::placeAt(Quadrant q, int x, int y) {
    x += 3 * (to_underlying(q) / 2);
    y += 3 * (to_underlying(q) % 2);
    placeAt(x,y);
}
