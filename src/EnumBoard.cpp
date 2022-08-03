//
// Created by Tobias Baert on 27/07/2022.
//

#include "EnumBoard.h"

EnumBoard::EnumBoard() : mTurn(Colour::WHITE)
               , mGrid()
               , mQuadrants()
               , mHasEnded(false) {}

void EnumBoard::reset() {
    mTurn = Colour::WHITE;
    mGrid.fill({});
    syncQuadrantsFromGrid();
    syncVictoryData();
}

void EnumBoard::syncGridFromQuadrants() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mGrid[i]    [j]     = mQuadrants[to_underlying(Quadrant::NORTHWEST)][i][j];
            mGrid[i]    [j + 3] = mQuadrants[to_underlying(Quadrant::NORTHEAST)][i][j];
            mGrid[i + 3][j]     = mQuadrants[to_underlying(Quadrant::SOUTHWEST)][i][j];
            mGrid[i + 3][j + 3] = mQuadrants[to_underlying(Quadrant::SOUTHEAST)][i][j];
        }
    }
}

void EnumBoard::syncQuadrantsFromGrid() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mQuadrants[to_underlying(Quadrant::NORTHWEST)][i][j] = mGrid[i]    [j];
            mQuadrants[to_underlying(Quadrant::NORTHEAST)][i][j] = mGrid[i]    [j + 3];
            mQuadrants[to_underlying(Quadrant::SOUTHWEST)][i][j] = mGrid[i + 3][j];
            mQuadrants[to_underlying(Quadrant::SOUTHEAST)][i][j] = mGrid[i + 3][j + 3];
        }
    }
}

Colour EnumBoard::getTurn() const {
    return mTurn;
}

IBoard::OptionalColour EnumBoard::getColourAt(int x, int y) const {
    assert(isValidGlobalCoord(x, y));
    return mGrid[x][y];
}


IBoard::OptionalColour EnumBoard::getColourAt(Quadrant q, int x, int y) const {
    assert(isValidQuadrantCoord(x,y));
    return mQuadrants[to_underlying(q)][x][y];
}

void EnumBoard::placeAt(int x, int y) {
    assert(isValidGlobalCoord(x, y));
    mGrid[x][y] = mTurn;
    syncQuadrantsFromGrid();
}

void EnumBoard::placeAt(Quadrant q, int x, int y) {
    mQuadrants[to_underlying(q)][x][y] = mTurn;
    syncGridFromQuadrants();
}

void EnumBoard::rotate(Quadrant q, RotationDir d) {
    switch (d) {
        case RotationDir::CLOCKWISE:
            transpose(mQuadrants[to_underlying(q)]);
            reverseRows(mQuadrants[to_underlying(q)]);
            break;
        case RotationDir::COUNTERCLOCKWISE:
            reverseRows(mQuadrants[to_underlying(q)]);
            transpose(mQuadrants[to_underlying(q)]);
            break;
    }
    syncGridFromQuadrants();
}

void EnumBoard::advanceTurn() {
    mTurn = (to_underlying(mTurn) ? Colour::BLACK : Colour::WHITE);
}

bool EnumBoard::hasEnded() const {
    return mHasEnded;
}

IBoard::OptionalColour EnumBoard::getWinner() const {
    OptionalColour result;

    if (mHasWinningPosition[to_underlying(Colour::WHITE)]) result = Colour::WHITE;
    if (mHasWinningPosition[to_underlying(Colour::BLACK)]) result = Colour::BLACK;
    if (mHasWinningPosition.all()) result = {std::nullopt};

    return result;
}

void EnumBoard::syncVictoryData() {
    mHasWinningPosition.reset();

    checkHorizontal();
    checkVertical();
    checkPriDiagonal();
    checkSecDiagonal();

    mHasEnded = mHasWinningPosition.any() || isFullyOccupied();
}

bool EnumBoard::isFullyOccupied() {
    return std::all_of(mGrid[0].begin(), mGrid[5].end(), [](auto e){return e.has_value();});
}

void EnumBoard::checkHorizontal() {
    static constexpr OffsetArray offsets {IntPair{0,0}, IntPair{0,1}, IntPair{0,2}, IntPair{0,3}, IntPair{0,4}};
    static const IntPairVector origins {
            IntPair{0,0}, IntPair{0,1},
            IntPair{1,0}, IntPair{1,1},
            IntPair{2,0}, IntPair{2,1},
            IntPair{3,0}, IntPair{3,1},
            IntPair{4,0}, IntPair{4,1},
            IntPair{5,0}, IntPair{5,1}
    };
    return checkSeries(origins, offsets);
}

void EnumBoard::checkVertical() {
    static constexpr OffsetArray offsets {IntPair{0,0}, IntPair{1,0}, IntPair{2,0}, IntPair{3,0}, IntPair{4,0}};
    static const IntPairVector origins {
            IntPair{0,0}, IntPair{0,1}, IntPair{0,2}, IntPair{0,3}, IntPair{0,4}, IntPair{0,5},
            IntPair{1,0}, IntPair{1,1}, IntPair{1,2}, IntPair{1,3}, IntPair{1,4}, IntPair{0,5},
    };
    return checkSeries(origins, offsets);
}

void EnumBoard::checkPriDiagonal() {
    static constexpr OffsetArray offsets {IntPair{0,0}, IntPair{1,1}, IntPair{2,2}, IntPair{3,3}, IntPair{4,4}};
    static const IntPairVector origins {
            IntPair{0,0}, IntPair{0,1},
            IntPair{1,0}, IntPair{1,1},
    };
    return checkSeries(origins, offsets);
}

void EnumBoard::checkSecDiagonal() {
    static constexpr OffsetArray offsets {IntPair{0,0}, IntPair{-1,-1}, IntPair{-2,-2}, IntPair{-3,-3}, IntPair{-4,-4}};
    static const IntPairVector origins {
            IntPair{4,0}, IntPair{4,1},
            IntPair{5,0}, IntPair{5,1}
    };
    return checkSeries(origins, offsets);

}

void EnumBoard::checkSeries(const IntPairVector& origins, const OffsetArray& offsets) {
    std::array<OptionalColour, 5> s; // to store the sequence of 5 cells under investigation
    for (auto & o : origins) {
        // Fill in s correctly: origin determines starting point, offsets determine direction.
        for (int i = 0; i < 5; i++) s[i] = mGrid[o.first + offsets[i].first][o.second + offsets[i].second];
        bool series =
                std::all_of(s.cbegin(), s.cend(), [] (auto& opt) {return opt.has_value();})  // all filled
                &&  std::all_of(s.cbegin(), s.cend(), [&s] (auto& opt) {return *opt == *s[0];}); // all same colour
        // If a series was found, set the corresponding flag: index 1 for WHITE, index 0 for BLACK
        if (series) mHasWinningPosition.set(to_underlying(*s[0]));
    }
}


bool EnumBoard::isValidGlobalCoord(int x, int y) {
    return 0 <= x && x < 6 && 0 <= y && y < 6;
}

bool EnumBoard::isValidQuadrantCoord(int x, int y) {
    return 0 <= x && x < 3 && 0 <= y && y < 3;
}

void EnumBoard::reverseRows(QuadrantGrid q) {
    std::swap(q[0], q[2]);
}

void EnumBoard::transpose(QuadrantGrid q) {
    std::swap(q[0][1], q[1][0]);
    std::swap(q[1][2], q[2][1]);
    std::swap(q[0][2], q[2][0]);
}
