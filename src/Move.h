//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_MOVE_H_DATE_27_07_2022_TIME_10_49
#define PENTAGO_MOVE_H_DATE_27_07_2022_TIME_10_49



class Move {
public:

    [[nodiscard]] short getX() const;

    [[nodiscard]] short getY() const;

    [[nodiscard]] Quadrant getRotated() const;

    [[nodiscard]] RotationDir getRotationDir() const;

private:
    // placement of new ball
    short x;
    short y;

    Quadrant rotated;
    RotationDir rotationDir;

};


#endif //PENTAGO_MOVE_H_DATE_27_07_2022_TIME_10_49
