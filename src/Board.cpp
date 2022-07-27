//
// Created by Tobias Baert on 27/07/2022.
//

#include "Board.h"

#include <algorithm>

Board::Board() : fullGrid()
               , northWestQuadrant()
               , northEastQuadrant()
               , southWestQuadrant()
               , southEastQuadrant() {}

void Board::generateMoves(std::vector<const Move> MoveVec) {

}

std::ostream& Board::operator<<(std::ostream& os) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            os << fullGrid[i * 6 + j] << ' ';
        }
    }
    return os;
}

void Board::synchroniseRepresentations() {

}
