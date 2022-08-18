//
// Created by Tobias Baert on 18/08/2022.
//

#ifndef PENTAGO_PLACEMENTSTATE_H_DATE_18_08_2022_TIME_15_42
#define PENTAGO_PLACEMENTSTATE_H_DATE_18_08_2022_TIME_15_42


#include <SFML/Graphics/RenderWindow.hpp>
#include <optional>

#include "IState.h"

class PlacementState : public IState {
public:
    explicit PlacementState(Game& game);

    void processEvent(sf::Event e) override;

private:
    sf::Vector2f pressStartPos;
    sf::Vector2f pressEndPos;

    void processLMB();

};


#endif //PENTAGO_PLACEMENTSTATE_H_DATE_18_08_2022_TIME_15_42
