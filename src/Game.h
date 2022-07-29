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

    void renderAllQuadrants(sf::Transform t);
    void renderSingleQuadrant(Quadrant q, sf::Transform t);

    void renderAllCellsForQuadrant(Quadrant q, sf::Transform t);
    void renderSingleCellForQuadrant(Quadrant q, int row, int col, sf::Transform t);

    static const int SCREEN_SIZE;

    static const float BOARD_MARGIN;
    static const float INTRA_QUADRANT_MARGIN;
    static const float CELL_SIZE;
    static const float QUADRANT_SIZE;

    sf::Color getSFColorAt(Quadrant q, int row, int ˆcol);
};


#endif //PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29
