//
// Created by Tobias Baert on 27/07/2022.
//

#include "Move.h"

short Move::getX() const {
    return x;
}

short Move::getY() const {
    return y;
}

Quadrant Move::getRotated() const {
    return rotated;
}

RotationDir Move::getRotationDir() const {
    return rotationDir;
}
