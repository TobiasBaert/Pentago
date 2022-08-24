//
// Created by Tobias Baert on 29/07/2022.
//

#include "BitBoardRepresentation.h"
#include "IBoard.h"

using namespace Enums;

BitBoardRepresentation::BitBoardRepresentation()
    : mTurn(Colour::WHITE)
    , mPhase(TurnPhase::PLACEMENT)
    , mWhiteHasWinningPosition(false)
    , mBlackHasWinningPosition(false) {}


void BitBoardRepresentation::reset() {
    mTurn = Colour::WHITE;
    mPhase = TurnPhase::PLACEMENT;
    mWhite.reset();
    mBlack.reset();
    syncDerivedData();
}

Colour BitBoardRepresentation::getTurn() const {
    return mTurn;
}

TurnPhase BitBoardRepresentation::getTurnPhase() const {
    return mPhase;
}


GamePhase BitBoardRepresentation::getGamePhase() const {
    static constexpr GamePhase array[2][2] = {
            {GamePhase::PLAYING,        GamePhase::BLACK_VICTORY},
            {GamePhase::WHITE_VICTORY,  GamePhase::DRAW         }
    };

    return array[mWhiteHasWinningPosition || mOccupancy.all()][mBlackHasWinningPosition || mOccupancy.all()];
}

IBoard::OptionalColour BitBoardRepresentation::getColourAt(size_t row, size_t col) const {
    size_t index = getIndexFrom(row, col);
    if (mWhite[index]) return Colour::WHITE;
    if (mBlack[index]) return Colour::BLACK;
    return {std::nullopt};
}

void BitBoardRepresentation::placeAt(size_t row, size_t col) {
    assert(mPhase == TurnPhase::PLACEMENT);
    assert(areValidGlobalCoords(row, col));
    if (mOccupancy[getIndexFrom(row, col)]) return;

    mColours[to_underlying(mTurn)].set(getIndexFrom(row, col));

    syncDerivedData();
    advancePhase();
}

void BitBoardRepresentation::rotate(Quadrant q, RotationDir d) {
    static constexpr void (BitBoardRepresentation::*table[2]) (Quadrant q) = {
            &BitBoardRepresentation::rotateQuadrant90Clockwise,
            &BitBoardRepresentation::rotateQuadrant90CounterClockwise
    };

    (this->*(table[to_underlying(d)]))(q);

    syncDerivedData();
    advancePhase();
    advanceTurn();
}

void BitBoardRepresentation::advanceTurn() {
    mTurn = !mTurn;
}

void BitBoardRepresentation::advancePhase() {
    mPhase = !mPhase;
}

void BitBoardRepresentation::syncDerivedData() {
    mOccupancy = mWhite | mBlack;
    mWhiteHasWinningPosition = hasWinningPosition(mWhite);
    mBlackHasWinningPosition = hasWinningPosition(mBlack);
}

size_t BitBoardRepresentation::getIndexFrom(size_t row, size_t col) {
    return (5 - row) * 6 + col; // using Little-Endian Rank-File Mapping
}

bool BitBoardRepresentation::hasWinningPosition(BitBoard x) { // 36 ops + check
    return ((x & (x >> 1) & (x >>  2) & (x >>  3) & (x >>  4) & BitBoard(0030303030303)) |
            (x & (x << 6) & (x << 12) & (x << 18) & (x << 24) & BitBoard(0777700000000)) |
            (x & (x >> 7) & (x >> 14) & (x >> 21) & (x >> 28) & BitBoard(0000000000303)) |
            (x & (x << 5) & (x << 10) & (x << 15) & (x << 20) & BitBoard(0030300000000)) ).any();
}

void BitBoardRepresentation::rotateQuadrant90Clockwise(Quadrant q) {
    reflectQuadrantHorizontally(q);
    reflectQuadrantDiagonally(q);
}

void BitBoardRepresentation::rotateQuadrant90CounterClockwise(Quadrant q) {
    reflectQuadrantDiagonally(q);
    reflectQuadrantHorizontally(q);
}

void BitBoardRepresentation::reflectQuadrantHorizontally(Quadrant q) {
    static constexpr BitBoard swap1Masks[4] = { // bottom row for every quadrant
            BitBoard(0000007000000), BitBoard(0000070000000),
            BitBoard(0000000000007), BitBoard(0000000000070)
    };

    const size_t index = to_underlying(q);

    deltaSwapInPlace(mWhite, swap1Masks[index], 12); // flip with top row of quadrant
    deltaSwapInPlace(mBlack, swap1Masks[index], 12);
}

void BitBoardRepresentation::reflectQuadrantDiagonally(Quadrant q) {
    static constexpr BitBoard swap2Masks[4] = { // first sub-diagonal for every quadrant
            BitBoard(0000102000000), BitBoard(0001020000000),
            BitBoard(0000000000102), BitBoard(0000000001020)
    };

    static constexpr BitBoard swap3Masks[4] = { // second sub-diagonal for every quadrant, i.e. bottom left corner
            BitBoard(0000001000000), BitBoard(0000010000000),
            BitBoard(0000000000001), BitBoard(0000000000010)
    };

    const size_t index = to_underlying(q);

    deltaSwapInPlace(mWhite, swap2Masks[index],  7); // swap first sub- and super diagonals
    deltaSwapInPlace(mWhite, swap3Masks[index], 14); // swap bottom left and upper right corners

    deltaSwapInPlace(mBlack, swap2Masks[index],  7);
    deltaSwapInPlace(mBlack, swap3Masks[index], 14);
}

void BitBoardRepresentation::deltaSwapInPlace(BitBoard& x, BitBoard select, size_t delta) {
    BitBoard t = x ^ (x >> delta) & select;
    x ^= (t << delta) ^ t;
}
