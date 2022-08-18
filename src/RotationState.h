//
// Created by Tobias Baert on 18/08/2022.
//

#ifndef PENTAGO_ROTATIONSTATE_H_DATE_18_08_2022_TIME_23_22
#define PENTAGO_ROTATIONSTATE_H_DATE_18_08_2022_TIME_23_22


#include "IState.h"

class RotationState : public IState {
public:
    explicit RotationState(Game& game);

    void processEvent(sf::Event e) override;

private:
    sf::Vector2f pressStartPos;
    sf::Vector2f pressEndPos;

    void processLMB();

};


#endif //PENTAGO_ROTATIONSTATE_H_DATE_18_08_2022_TIME_23_22
