//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29
#define PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29


#include <memory>
#include <SFML/Graphics.hpp>

#include "IBoard.h"
#include "EnumBoard.h"

class Game {
public:

    Game();

    void run();

private:
    std::unique_ptr<IBoard> pBoard = std::unique_ptr<IBoard>(new EnumBoard());
    sf::RenderWindow mWindow;

    void processEvents();
    void render();

};


#endif //PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29
