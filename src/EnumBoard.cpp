//
// Created by Tobias Baert on 27/07/2022.
//

#include "EnumBoard.h"

EnumBoard::EnumBoard() : mTurn(WHITE)
               , mGrid()
               , mNorthWestQuadrant()
               , mNorthEastQuadrant()
               , mSouthWestQuadrant()
               , mSouthEastQuadrant() {}

void EnumBoard::syncGridFromQuadrants() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mGrid[i]  [j]    = mNorthWestQuadrant[i][j];
            mGrid[i]  [j + 3]  = mNorthEastQuadrant[i][j];
            mGrid[i + 3][j]    = mSouthWestQuadrant[i][j];
            mGrid[i + 3][j + 3]  = mSouthEastQuadrant[i][j];
        }
    }
}

void EnumBoard::syncQuadrantsFromGrid() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mNorthWestQuadrant[i][j] = mGrid[i]  [j];
            mNorthEastQuadrant[i][j] = mGrid[i]  [j + 3];
            mSouthWestQuadrant[i][j] = mGrid[i + 3][j];
            mSouthEastQuadrant[i][j] = mGrid[i + 3][j + 3];
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

}

void EnumBoard::advanceTurn() {
    mTurn = (mTurn ? BLACK : WHITE);
}

IBoard::OptionalColour EnumBoard::getWinner() {
    return {}; // TODO
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
