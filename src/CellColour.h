//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_CELLCOLOUR_H_DATE_27_07_2022_TIME_10_09
#define PENTAGO_CELLCOLOUR_H_DATE_27_07_2022_TIME_10_09

#include <ostream>
#include "PlayerColour.h"

class CellColour : public PlayerColour {
public:
    CellColour() : PlayerColour(0) {} // Default ctor added for easy zero-init of quadrant arrays
    static CellColour Empty() { return CellColour(0); }

protected:
    explicit CellColour(short v) : PlayerColour(v) {}

}

#endif //PENTAGO_CELLCOLOUR_H_DATE_27_07_2022_TIME_10_09
