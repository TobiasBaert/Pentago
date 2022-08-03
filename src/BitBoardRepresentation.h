//
// Created by Tobias Baert on 29/07/2022.
//

#ifndef PENTAGO_BITBOARDREPRESENTATION_H_DATE_29_07_2022_TIME_17_34
#define PENTAGO_BITBOARDREPRESENTATION_H_DATE_29_07_2022_TIME_17_34

#include <bitset>

#include "IBoard.h"

class BitBoardRepresentation : public IBoard {
public:
    BitBoardRepresentation();

    void reset() override;

    [[nodiscard]] Colour getTurn() const override;

    [[nodiscard]] OptionalColour getColourAt(int x, int y) const override;

    void placeAt(int x, int y) override;

    void rotate(Quadrant q, RotationDir d) override;

    void advanceTurn() override;

    [[nodiscard]] bool hasEnded() const override;

    [[nodiscard]] OptionalColour getWinner() const override;

private:
    using BitBoard = std::bitset<36>;

    /// Board state
    Colour mTurn;
    BitBoard mColours[2];
    BitBoard& mWhite = mColours[to_underlying(Colour::WHITE)];
    BitBoard& mBlack = mColours[to_underlying(Colour::BLACK)];

    /// Derived boards
    BitBoard mOccupancy;

    /// Victory detection
    bool mHasEnded;
    bool mWhiteHasWinningPosition;
    bool mBlackHasWinningPosition;

    inline void syncDerivedFields();

    /// Rotations
    inline void rotateQuadrant90Clockwise(Quadrant q);
    inline void rotateQuadrant90CounterClockwise(Quadrant q);
    inline void reflectQuadrantHorizontally(Quadrant q);
    inline void reflectQuadrantDiagonally(Quadrant q);

    /// Utilities
    inline static size_t getIndexFrom(int row, int col);
    inline static bool hasWinningPosition(BitBoard x);
    inline static void deltaSwapInPlace(BitBoard& x, BitBoard select, size_t delta);
};


#endif //PENTAGO_BITBOARDREPRESENTATION_H_DATE_29_07_2022_TIME_17_34
