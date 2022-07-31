//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_BOARD_H_DATE_27_07_2022_TIME_09_47
#define PENTAGO_BOARD_H_DATE_27_07_2022_TIME_09_47

#include "IBoard.h"

#include <array>
#include <bitset>
#include <vector>

class EnumBoard : public IBoard {
public:

    EnumBoard();

    void reset() override;

    [[nodiscard]] Colour getTurn() const override;

    [[nodiscard]] OptionalColour getColourAt(int x, int y) const override;

    [[nodiscard]] OptionalColour getColourAt(Quadrant q, int x, int y) const override;

    void placeAt(Colour col, int x, int y) override;

    void rotate(Quadrant q, RotationDir d) override;

    void advanceTurn() override;

    [[nodiscard]] bool hasEnded() const override;

    [[nodiscard]] OptionalColour getWinner() const override;

private:

    /// Board state
    using Grid = std::array<std::array<OptionalColour, 6>, 6>;
    using QuadrantGrid = std::array<std::array<OptionalColour, 3>, 3>;

    Colour mTurn;
    Grid mGrid;
    std::array<QuadrantGrid, 4> mQuadrants;

    /// Victory detection
    bool mHasEnded;
    std::bitset<2> mHasWinningPosition;

    void syncVictoryData();

    using IntPair = std::pair<int,int>;
    using IntPairVector = std::vector<IntPair>;
    using OffsetArray = std::array<IntPair,5>;

    bool isFullyOccupied();

    void checkHorizontal();
    void checkVertical();
    void checkPriDiagonal();
    void checkSecDiagonal();
    void checkSeries(const IntPairVector& origins, const OffsetArray& offsets);

    /// Synchronisation
    void syncGridFromQuadrants();
    void syncQuadrantsFromGrid();

    /// Utilities
    static bool isValidGlobalCoord(int x, int y);
    static bool isValidQuadrantCoord(int x, int y);

    static void reverseRows(QuadrantGrid q);
    static void transpose(QuadrantGrid q);

};


#endif //PENTAGO_BOARD_H_DATE_27_07_2022_TIME_09_47
