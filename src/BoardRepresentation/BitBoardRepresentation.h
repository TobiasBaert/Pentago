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

    [[nodiscard]] Enums::Colour getTurn() const override;

    [[nodiscard]] Enums::TurnPhase getTurnPhase() const override;

    [[nodiscard]] OptionalColour getColourAt(size_t row, size_t col) const override;

    void placeAt(size_t row, size_t col) override;

    void rotate(Enums::Quadrant q, Enums::RotationDir d) override;

    [[nodiscard]] bool hasEnded() const override;

    [[nodiscard]] OptionalColour getWinner() const override;

private:
    using BitBoard = std::bitset<36>;

    /// Board state
    Enums::Colour mTurn;
    Enums::TurnPhase mPhase;

    BitBoard mColours[2];
    BitBoard& mWhite = mColours[Enums::to_underlying(Enums::Colour::WHITE)];
    BitBoard& mBlack = mColours[Enums::to_underlying(Enums::Colour::BLACK)];

    /// Derived boards
    BitBoard mOccupancy;

    /// Victory detection
    bool mHasEnded;
    bool mWhiteHasWinningPosition;
    bool mBlackHasWinningPosition;

    inline void syncDerivedData();
    inline void advanceTurn();
    inline void advancePhase();

    /// Rotations
    inline void rotateQuadrant90Clockwise(Enums::Quadrant q);
    inline void rotateQuadrant90CounterClockwise(Enums::Quadrant q);
    inline void reflectQuadrantHorizontally(Enums::Quadrant q);
    inline void reflectQuadrantDiagonally(Enums::Quadrant q);

    /// Utilities
    inline static size_t getIndexFrom(size_t row, size_t col);
    inline static bool hasWinningPosition(BitBoard x);
    inline static void deltaSwapInPlace(BitBoard& x, BitBoard select, size_t delta);
};


#endif //PENTAGO_BITBOARDREPRESENTATION_H_DATE_29_07_2022_TIME_17_34
