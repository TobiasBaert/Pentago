//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_BOARD_H_DATE_27_07_2022_TIME_09_47
#define PENTAGO_BOARD_H_DATE_27_07_2022_TIME_09_47

#include <vector>

#include "CellColour.h"
#include "Move.h"

class Board {
public:

    Board();

    void generateMoves(std::vector<const Move> MoveVec);

    std::ostream& operator<<(std::ostream& os);

private:
    CellColour fullGrid[6][6];
    CellColour northWestQuadrant[3][3];
    CellColour northEastQuadrant[3][3];
    CellColour southWestQuadrant[3][3];
    CellColour southEastQuadrant[3][3];

    void synchroniseRepresentations();
};


#endif //PENTAGO_BOARD_H_DATE_27_07_2022_TIME_09_47
