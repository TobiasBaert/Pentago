//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29
#define PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29


#include <SFML/Graphics/RenderWindow.hpp>

class Game {
public:

    explicit Game(unsigned int screensize);

    void run();

private:
    sf::RenderWindow window;

};


#endif //PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29
