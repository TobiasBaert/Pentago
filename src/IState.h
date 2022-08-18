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

    virtual void processEvent(sf::Event e) = 0;

protected:
    Game& rGame;

    static sf::Vector2f toVec2f(sf::Vector2i p) {
        return {static_cast<float>(p.x), static_cast<float>(p.y)};
    }
};


#endif //PENTAGO_ISTATE_H_DATE_18_08_2022_TIME_15_40
