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

    [[nodiscard]] Colour getTurn() const override;

    [[nodiscard]] OptionalColour colourAt(int x, int y) const override;

    void placeAt(Colour col, int x, int y) override;

    void rotate(Quadrant q, RotationDir d) override;

    void advanceTurn() override;

    [[nodiscard]] OptionalColour getWinner() const override;

private:
    Colour mTurn;

    OptionalColour mGrid[6][6];
    OptionalColour mQuadrants[4][3][3];

    using IntPair = std::pair<int,int>;
    using IntPairVector = std::vector<IntPair>;
    using OffsetArray = std::array<IntPair,5>;
    [[nodiscard]] std::bitset<2> checkSeries(const IntPairVector& origins, const OffsetArray& offsets) const;
    [[nodiscard]] std::bitset<2> checkHorizontal() const;
    [[nodiscard]] std::bitset<2> checkVertical() const;
    [[nodiscard]] std::bitset<2> checkPriDiagonal() const;
    [[nodiscard]] std::bitset<2> checkSecDiagonal() const;

    void syncGridFromQuadrants();
    void syncQuadrantsFromGrid();

    static bool isValidCoord(int x, int y);

    static void reverseRows(OptionalColour q[3][3]);
    static void transpose(OptionalColour q[3][3]);
};


#endif //PENTAGO_BOARD_H_DATE_27_07_2022_TIME_09_47
