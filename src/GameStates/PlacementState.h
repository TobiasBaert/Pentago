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
    /**
     * Initialises the placement state as a state of the given game.
     * @param game the game this state relates to
     */
    explicit PlacementState(Game& game);

    void processInputs() override;

private:
    /**
     * The two possible states for the left mouse button (LMB).
     *
     * In the IDLE state, the values for pressStartPos has no meaning and should not be used.
     */
    enum class LMBState {
        IDLE,
        HELD,
    };

    /**
     * Current LMB state.
     */
    LMBState mLMBState = LMBState::IDLE;

    /**
     * The position, relative to the window, where the mouse is initially pressed.
     */
    sf::Vector2f pressStartPos;

    /**
     * Handle the mouse action that consists of pressing the LMB at the stored, initial position and depressing the
     * LMB at the given position.
     * @param pressEndPos the position where the LMB is lifted.
     */
    void processLMB(sf::Vector2f pressEndPos);

};


#endif //PENTAGO_PLACEMENTSTATE_H_DATE_18_08_2022_TIME_15_42
