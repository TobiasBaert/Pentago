//
// Created by Tobias Baert on 27/07/2022.
//

#include "Board.h"

#include <algorithm>

Board::Board() : turn(PlayerColour::White())
               , fullGrid()
               , northWestQuadrant()
               , northEastQuadrant()
               , southWestQuadrant()
               , southEastQuadrant() {}

void Board::syncGridFromQuadrants() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            fullGrid[i]  [j]    = northWestQuadrant[i][j];
            fullGrid[i]  [j+3]  = northEastQuadrant[i][j];
            fullGrid[i+3][j]    = southWestQuadrant[i][j];
            fullGrid[i+3][j+3]  = southEastQuadrant[i][j];
        }
    }
}

void Board::syncQuadrantsFromGrid() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            northWestQuadrant[i][j] = fullGrid[i]  [j];
            northEastQuadrant[i][j] = fullGrid[i]  [j+3];
            southWestQuadrant[i][j] = fullGrid[i+3][j];
            southEastQuadrant[i][j] = fullGrid[i+3][j+3];
        }
    }
}
