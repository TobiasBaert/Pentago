//
// Created by Tobias Baert on 27/07/2022.
//

#include "IBoard.h"

IBoard::OptionalColour IBoard::getColourAt(Enums::Quadrant q, size_t row, size_t col) const {
    row += 3 * (to_underlying(q) / 2);
    col += 3 * (to_underlying(q) % 2);
    return getColourAt(row, col);
}

void IBoard::placeAt(Enums::Quadrant q, size_t row, size_t col) {
    row += 3 * (to_underlying(q) / 2);
    col += 3 * (to_underlying(q) % 2);
    placeAt(row,col);
}

bool IBoard::areValidGlobalCoords(size_t x, size_t y) {
    return 0 <= x && x < 6 && 0 <= y && y < 6;
}

bool IBoard::areValidQuadrantCoords(size_t x, size_t y) {
    return 0 <= x && x < 3 && 0 <= y && y < 3;
}