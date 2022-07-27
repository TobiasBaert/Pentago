//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29
#define PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29


#include "View.h"
#include "EnumBoard.h"

class Game {
public:
    void run();

private:
    View view;
    EnumBoard board;
};


#endif //PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29
