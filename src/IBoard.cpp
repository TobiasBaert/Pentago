//
// Created by Tobias Baert on 27/07/2022.
//

#include "IBoard.h"

bool IBoard::isEmpty(int x, int y) {
    return !colourAt(x,y).has_value();
}

bool IBoard::isWhite(int x, int y) {
    return *colourAt(x,y);
}

bool IBoard::isBlack(int x, int y) {
    return !*colourAt(x,y);
}

bool IBoard::isWon() {
    return getWinner().has_value();
}

bool IBoard::whiteToPlay() {
    return getTurn() == WHITE;
}

bool IBoard::blackToPlay() {
    return getTurn() == BLACK;
}
