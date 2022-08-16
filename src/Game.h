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
protected:
    using OptionalQuadrant = std::optional<Quadrant>;
    using OptionalIntPair = std::optional<std::pair<int,int>>;
    using CoordinateTuple = std::tuple<OptionalQuadrant, OptionalIntPair>;

public:

    /**
     * Initialises the game.
     *
     * This also initialises a board representation and a window.
     */
    Game();

    /**
     * Starts the game loop of detecting inputs and drawing the game.
     */
    void run();

    /**
     * Checks whether a given position in the window's coordinate system is located within any of the quadrants and
     * if so, if it is located within any of that quadrant's cells.
     * @param p the position in the window coordinate system
     * @return A pair of optionals. If the position is within a quadrant, then the first value is that quadrant,
     * otherwise it is empty. If the position is additionally within a cell within that quadrant, then the second
     * value is also a pair that represents the row and column (in range [0,2]) within the quadrant. Otherwise the
     * second value is empty. If the first value is empty, the second value will always be empty too.
     */
    CoordinateTuple getCoordinateTupleFromPosition(sf::Vector2f p) const;

private:
    /**
     * The link to the board representation.
     */
    std::unique_ptr<IBoard> pBoard = std::unique_ptr<IBoard>(new EnumBoard());

    /// All kinds of layout related constants. TODO move to config file/namespace
    static constexpr int   SCREEN_SIZE              = 1200;
    static constexpr float CELL_SIZE                = 150.f;
    static constexpr float QUADRANT_SIZE            = 3 * CELL_SIZE;
    static constexpr float INTER_QUADRANT_MARGIN    = 15.f;
    static constexpr float CIRCLE_RADIUS            = 0.6f * CELL_SIZE / 2;
    static constexpr float QUADRANT_CENTRE_OFFSET   = QUADRANT_SIZE / 2 + INTER_QUADRANT_MARGIN;

    const sf::Vector2f SCREEN_CENTRE    {SCREEN_SIZE / 2.f  , SCREEN_SIZE / 2.f};
    const sf::Vector2f QUADRANT_CENTRE  {QUADRANT_SIZE / 2.f, QUADRANT_SIZE / 2.f};
    const sf::Vector2f CIRCLE_CENTRE    {CIRCLE_RADIUS      , CIRCLE_RADIUS};

    /**
     * The window everything is drawn to.
     */
    sf::RenderWindow mWindow{sf::VideoMode(SCREEN_SIZE, SCREEN_SIZE), "Pentago",
                             sf::Style::Close | sf::Style::Titlebar};

    /**
     * The rounded rectangle shape that is used to draw the quadrants. This shape is reused for every quadrant.
     */
    RoundedRectangleShape<10> mQuadrantShape {{QUADRANT_SIZE, QUADRANT_SIZE}, 0.1f * QUADRANT_SIZE};

    /**
     * The circle shape that is used to draw the cells. This shape is reused for every cell.
     */
    sf::CircleShape mCellShape {CIRCLE_RADIUS};

    /**
     * An array that contains for every quadrant the transform that starts at the starts at the upper-left corner of
     * the window (i.e., the origin of the global coordinate system) and ends in the centre of the quadrant.
     *
     * The array is intended to be indexed with a quadrant, and is ordered as such.
     */
    const std::array<sf::Transform, 4> mQuadrantTransforms {
        translation(SCREEN_CENTRE).translate({-QUADRANT_CENTRE_OFFSET, -QUADRANT_CENTRE_OFFSET}),
        translation(SCREEN_CENTRE).translate({ QUADRANT_CENTRE_OFFSET, -QUADRANT_CENTRE_OFFSET}),
        translation(SCREEN_CENTRE).translate({-QUADRANT_CENTRE_OFFSET,  QUADRANT_CENTRE_OFFSET}),
        translation(SCREEN_CENTRE).translate({ QUADRANT_CENTRE_OFFSET,  QUADRANT_CENTRE_OFFSET}),
    };

    /**
     * A 2-dimensional array that contains for every cell in a quadrant the transform that starts at the centre of the
     * quadrant and ends in the centre of the cell.
     *
     * Element [i][j] points the centre of the cell at row i and column j.
     */
    const std::array<std::array<sf::Transform, 3>, 3> mCellTransforms {{
        {translation({-CELL_SIZE,-CELL_SIZE}),   translation({0, -CELL_SIZE}),   translation({CELL_SIZE, -CELL_SIZE})},
        {translation({-CELL_SIZE, 0}),           translation({0, 0}),            translation({CELL_SIZE, 0})},
        {translation({-CELL_SIZE, CELL_SIZE}),   translation({0, CELL_SIZE}),    translation({CELL_SIZE, CELL_SIZE})}
    }};

    /**
     * The order the quadrants will be rendered in. Of particular importance is the last position, as this will be
     * drawn on top of all the others.
     */
    std::array<Quadrant, 4> mQuadrantRenderOrder = {
            Quadrant::NORTHWEST, Quadrant::NORTHEAST,
            Quadrant::SOUTHWEST, Quadrant::SOUTHEAST
    };

    /**
     * Configures the basic shape used to draw the quadrants. This centres the origin, sets the fill colour, outline
     * colour and outline thickness.
     */
    void configureQuadrantShapes();

    /**
     * Configures the basic shape used to draw the cells. This centres the origin, sets the fill colour, outline
     * colour and outline thickness.
     */
    void configureCellShapes();

    void processEvents();

    /**
     * Checks whether a given position in the window's coordinate system is located within any of the quadrants.
     * @param p the position in the window coordinate system
     * @param pInQuadrantLocalCoordSystem OUT argument, if the given position is within a quadrant Q, then this
     * position is set to the given position expressed in the coordinate system of Q. Otherwise, left unchanged.
     * @return if the given position is within a quadrant, then returns the quadrant. Otherwise, returns an empty
     * optional.
     */
    OptionalQuadrant getQuadrantFromPosition(sf::Vector2f p, sf::Vector2f& pInQuadrantLocalCoordSystem /*OUT*/) const;

    /**
     * Checks whether a given position in a quadrant's coordinate system is located within any of the cells.
     * @param pInQuadrantLocalCoordSystem the position in the quadrant's coordinate system.
     * @return if the given position is within the circle of a cell, then returns a pair of coordinates expressing the
     * row and col (in range [0,2]) of that cell. Otherwise, returns an empty optional.
     */
    OptionalIntPair getCellCoordsFromPosition(sf::Vector2f pInQuadrantLocalCoordSystem) const;

    /**
     * Draws all the quadrant and their cells.
     */
    void render();

    /**
     * Set a particular quadrant to be drawn last and thus on top of other quadrants.
     *
     * This is useful during the rotation of a quadrant to ensure that its corners are still visible.
     * @param q the quadrant that should be rendered last
     */
    void setQuadrantToRenderLast(Quadrant q);

    /**
     * Render a particular quadrant and all the cells therein.
     * @param q the quadrant to render
     */
    void renderQuadrant(Quadrant q);

    /**
     * Get the fill colour (in terms of the graphical framework) for the cell at a given row and column WITHIN the
     * given quadrant.
     * @param q the quadrant
     * @param row the row within the quadrant, in range [0,2]
     * @param col the column within the quadrant, in range [0,2]
     * @return if there is a marble in the cell at the given row and column within the quadrant, returns the SFML's
     * white or black as appropriate, otherwise return a darker version of the quadrant's red.
     */
    sf::Color getSFColorAt(Quadrant q, int row, int col);

    /**
     * Shorthand for creating translations.
     *
     * Simply a wrapper for sf::Transform().translate(t).
     * @param t the end point of the translation
     * @return returns an empty transform, translated by t.
     */
    static sf::Transform translation(sf::Vector2f t);
};


#endif //PENTAGO_GAME_H_DATE_27_07_2022_TIME_09_29
