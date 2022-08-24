//
// Created by Tobias Baert on 18/08/2022.
//

#ifndef PENTAGO_ISTATE_H_DATE_18_08_2022_TIME_15_40
#define PENTAGO_ISTATE_H_DATE_18_08_2022_TIME_15_40


#include <SFML/Window/Event.hpp>

class Game;

class IState {
public:
    explicit IState(Game& game) : rGame(game) {};

    virtual ~IState() = default;

    /**
     * Processes real-time inputs (specifically mouse clicks).
     */
    virtual void processInputs() = 0;

protected:
    Game& rGame;

    /**
     * Converts a vector of ints to a vector of floats.
     * @param p the vector to convert
     * @return a vector of floats that best approximates the original vector.
     */
    static sf::Vector2f toVec2f(sf::Vector2i p);

    /**
     * Shorthand for obtaining the position of the mouse, relative to the window.
     * @return the mouse position, relative to the window.
     */
    [[nodiscard]] sf::Vector2f getMousePosition() const;
};


#endif //PENTAGO_ISTATE_H_DATE_18_08_2022_TIME_15_40
