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

    [[nodiscard]] Enums::Colour getTurn() const override;

    [[nodiscard]] Enums::TurnPhase getTurnPhase() const override;

    [[nodiscard]] Enums::GamePhase getGamePhase() const override;

    [[nodiscard]] OptionalColour getColourAt(size_t row, size_t col) const override;

    [[nodiscard]] OptionalColour getColourAt(Enums::Quadrant q, size_t row, size_t col) const override;

    void placeAt(Enums::Quadrant q, size_t row, size_t col) override;

    void placeAt(size_t row, size_t col) override;

    void rotate(Enums::Quadrant q, Enums::RotationDir d) override;

private:

    /// Board state
    using Grid = std::array<std::array<OptionalColour, 6>, 6>;
    using QuadrantGrid = std::array<std::array<OptionalColour, 3>, 3>;

    Enums::Colour mTurn;
    Enums::TurnPhase mPhase;

    void advanceTurn();
    void advancePhase();

    Grid mGrid;
    std::array<QuadrantGrid, 4> mQuadrants;

    /// Victory detection
    std::bitset<2> mHasWinningPosition;

    void syncVictoryData();

    using IntPair = std::pair<int,int>;
    using IntPairVector = std::vector<IntPair>;
    using OffsetArray = std::array<IntPair,5>;

    [[nodiscard]] bool isFullyOccupied() const;

    void checkHorizontal();
    void checkVertical();
    void checkPriDiagonal();
    void checkSecDiagonal();
    void checkSeries(const IntPairVector& origins, const OffsetArray& offsets);

    /// Synchronisation
    void syncGridFromQuadrants();
    void syncQuadrantsFromGrid();

    /// Utilities
    static void reverseRows(QuadrantGrid& q);
    static void transpose(QuadrantGrid& q);

};


#endif //PENTAGO_BOARD_H_DATE_27_07_2022_TIME_09_47
