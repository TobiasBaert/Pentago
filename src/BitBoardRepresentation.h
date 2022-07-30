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

    [[nodiscard]] Colour getTurn() const override;

    [[nodiscard]] OptionalColour getColourAt(int x, int y) const override;

    void placeAt(Colour col, int x, int y) override;

    void rotate(Quadrant q, RotationDir d) override;

    void advanceTurn() override;

    [[nodiscard]] bool hasEnded() const override;

    [[nodiscard]] OptionalColour getWinner() const override;

private:
    using BitBoard = std::bitset<36>;

    BitBoard mColours[2];
    BitBoard& mWhite = mColours[to_underlying(Colour::WHITE)];
    BitBoard& mBlack = mColours[to_underlying(Colour::BLACK)];

    /// Derived boards
    BitBoard mOccupancy;

    void syncDerivedBoards();

    Colour mTurn;

    bool mIsEnded;
    bool mWhiteHasWinningPosition;
    bool mBlackHasWinningPosition;

    constexpr static BitBoard quadrantMasks[4] = {
        BitBoard(0070707000000), BitBoard(0707070000000),
        BitBoard(0000000070707), BitBoard(0000000707070)
    };

    /// Utilities
    static size_t getIndexFrom(int row, int col);

    static bool hasWinningPosition(BitBoard x);

    void updateVictoryData();

    static void deltaSwapInPlace(BitBoard& x, BitBoard select, size_t delta);

    void flipQuadrant90Clockwise(Quadrant q);
    void flipQuadrant90CounterClockwise(Quadrant q);
    void flipQuadrantHorizontally(Quadrant q);
    void flipQuadrantDiagonally(Quadrant q);


};


#endif //PENTAGO_BITBOARDREPRESENTATION_H_DATE_29_07_2022_TIME_17_34
