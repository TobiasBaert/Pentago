//
// Created by Tobias Baert on 27/07/2022.
//

#include <array>
#include "EnumBoard.h"

EnumBoard::EnumBoard() : mTurn(Colour::WHITE)
               , mGrid()
               , mQuadrants() {}

void EnumBoard::syncGridFromQuadrants() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mGrid[i]    [j]     = mQuadrants[to_underlying(Quadrant::NORTHWEST)][i][j];
            mGrid[i]    [j + 3] = mQuadrants[to_underlying(Quadrant::NORTHEAST)][i][j];
            mGrid[i + 3][j]     = mQuadrants[to_underlying(Quadrant::SOUTHWEST)][i][j];
            mGrid[i + 3][j + 3] = mQuadrants[to_underlying(Quadrant::SOUTHEAST)][i][j];
        }
    }
}

void EnumBoard::syncQuadrantsFromGrid() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mQuadrants[to_underlying(Quadrant::NORTHWEST)][i][j] = mGrid[i]    [j];
            mQuadrants[to_underlying(Quadrant::NORTHEAST)][i][j] = mGrid[i]    [j + 3];
            mQuadrants[to_underlying(Quadrant::SOUTHWEST)][i][j] = mGrid[i + 3][j];
            mQuadrants[to_underlying(Quadrant::SOUTHEAST)][i][j] = mGrid[i + 3][j + 3];
        }
    }
}

Colour EnumBoard::getTurn() {
    return mTurn;
}

IBoard::OptionalColour EnumBoard::colourAt(int x, int y) {
    assert(isValidCoord(x, y));
    return mGrid[x][y];
}

void EnumBoard::placeAt(Colour col, int x, int y) {
    assert(isValidCoord(x,y));
    mGrid[x][y] = col;
    syncQuadrantsFromGrid();
}

void EnumBoard::rotate(Quadrant q, RotationDir d) {
    switch (d) {
        case RotationDir::CLOCKWISE:
            transpose(mQuadrants[to_underlying(q)]);
            reverseRows(mQuadrants[to_underlying(q)]);
            break;
        case RotationDir::COUNTERCLOCKWISE:
            reverseRows(mQuadrants[to_underlying(q)]);
            transpose(mQuadrants[to_underlying(q)]);
            break;
    }
    syncGridFromQuadrants();
}

void EnumBoard::advanceTurn() {
    mTurn = (to_underlying(mTurn) ? Colour::BLACK : Colour::WHITE);
}

IBoard::OptionalColour EnumBoard::getWinner() {
    OptionalColour optionalColour = mGrid[0][0];
    if(optionalColour) {
        Colour colour = *optionalColour;
        bool series = true;
        for (int i = 0; i < 4; i++) {
            series = (mGrid[0][0+i].has_value() && *mGrid[0][0+i] == colour);
        }

    }
}

IBoard::OptionalColour EnumBoard::hasWinnerHorizontal() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) { // consider first two columns
            OptionalColour cell = mGrid[i][j];
            if (cell) {
                Colour colour = *cell;
                bool series = true;
                for (int k = 0; series && k < 4; k++) {
                    series = mGrid[i][j + k].has_value() && *mGrid[i][j + k] == colour;
                }
                if (series) return colour;
            }
        }
    }
    return {std::nullopt};
}


IBoard::OptionalColour EnumBoard::hasWinnerVertical() {
    for (int i = 0; i < 2; i++) { // first two rows
        for (int j = 0; j < 6; j++) { // every column
            OptionalColour cell = mGrid[i][j];
            if (cell) {
                Colour colour = *cell;
                bool series = true;
                for (int k = 0; series && k < 4; k++) {
                    series = mGrid[i + k][j].has_value() && *mGrid[i + k][j] == colour;
                }
                if (series) return colour;
            }
        }
    }
    return {std::nullopt};
}

IBoard::OptionalColour EnumBoard::hasWinnerPriDiagonal() {
    return IBoard::OptionalColour();
}

IBoard::OptionalColour EnumBoard::hasWinnerSecDiagonal() {
    return IBoard::OptionalColour();
}

bool EnumBoard::isValidCoord(int x, int y) {
    return 0 <= x && x <= 6 && 0 <= y && y <= 6;
}

void EnumBoard::reverseRows(IBoard::OptionalColour (* q)[3]) {
    for (int i = 0; i < 3; i++) std::swap(*q, *(q+2));
}

void EnumBoard::transpose(IBoard::OptionalColour (* q)[3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::swap(*(q[i]+j), *(q[j]+i));
        }
    }
}
