//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29
#define PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29


#include <memory>
#include <SFML/Graphics.hpp>

#include "IBoard.h"
#include "EnumBoard.h"
#include "RoundedRectangleShape.hpp"
#include "Utilities.h"

class Game {
public:

    Game();

    void run();

private:
    std::unique_ptr<IBoard> pBoard = std::unique_ptr<IBoard>(new EnumBoard());

    static constexpr int SCREEN_SIZE = 1200;
    static constexpr float CELL_SIZE = 150.f;
    static constexpr float QUADRANT_SIZE = 3 * CELL_SIZE;
    static constexpr float INTRA_QUADRANT_MARGIN = 15.f;
    static constexpr float CIRCLE_RADIUS = 0.6f * CELL_SIZE / 2;


    sf::RenderWindow mWindow{sf::VideoMode(SCREEN_SIZE, SCREEN_SIZE), "Pentago",
                             sf::Style::Close | sf::Style::Titlebar};

    std::array<RoundedRectangleShape<5>, 4> mQuadrantShapes
        = Util::create_array<4, RoundedRectangleShape<5>>(
                {{QUADRANT_SIZE, QUADRANT_SIZE}, 0.1f * QUADRANT_SIZE});

    std::array<std::array<sf::CircleShape, 6>, 6> mCircleShapes {{
        Util::create_array<6>(sf::CircleShape{CIRCLE_RADIUS}),
        Util::create_array<6>(sf::CircleShape{CIRCLE_RADIUS}),
        Util::create_array<6>(sf::CircleShape{CIRCLE_RADIUS}),
        Util::create_array<6>(sf::CircleShape{CIRCLE_RADIUS}),
        Util::create_array<6>(sf::CircleShape{CIRCLE_RADIUS}),
        Util::create_array<6>(sf::CircleShape{CIRCLE_RADIUS}),
    }};

    void configureQuadrantShapes();
    void configureCellShapes();

    void processEvents();

    void render();
    void renderAllQuadrants(sf::Transform t);
    void renderSingleQuadrant(Quadrant q, sf::Transform t);
    void renderAllCellsForQuadrant(Quadrant q, sf::Transform t);
    void renderSingleCellForQuadrant(Quadrant q, int row, int col, sf::Transform t);

    sf::Color getSFColorAt(Quadrant q, int row, int col);
};


#endif //PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29
