//
// Created by Tobias Baert on 18/08/2022.
//

#ifndef PENTAGO_ROTATIONSTATE_H_DATE_18_08_2022_TIME_23_22
#define PENTAGO_ROTATIONSTATE_H_DATE_18_08_2022_TIME_23_22


#include <SFML/Graphics/Transform.hpp>
#include <tuple>

#include "IState.h"
#include "IBoard.h"

class RotationState : public IState {
public:
    /**
     * Initialises the rotation state as a state of the given game.
     * @param game the game this state relates to
     */
    explicit RotationState(Game& game);

    void processInputs() override;

private:
    /**
     * The two possible states for the left mouse button (LMB).
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
     * In the HELD state, if the LMB was pressed within a quadrant, then the first field refers to the position of
     * the initial LMB press and the second field refers to the last polled position of the mouse.
     * Otherwise, this value is empty.
     *
     * In effect, mStartAndEnd is empty iff the associated Game's mRotatingQuadrant is empty.
     */
    std::optional<std::pair<sf::Vector2f, sf::Vector2f>> mStartAndEnd;

    /**
     * Calculates the current mouse position relative to the centre of the actively rotated quadrant.
     * @pre mStartAndEnd is not empty.
     * @return the current mouse position, in the coordinate system centred in the quadrant in which the initial
     * click happened.
     */
    sf::Vector2f getRelativeMousePosition();

    /**
     * Sets a given quadrant as the actively rotating quadrant. This includes setting the initial rotation to zero and
     * setting the quadrant to be drawn last.
     * @param q the quadrant to be rotated
     */
    void setRotatingQuadrant(Enums::Quadrant q);

    /**
     * Adjusts the rotation of the actively rotating quadrant in correspondence with the difference in the current
     * mouse position and the stored initial LMB press position.
     * @pre mStartAndEnd is not empty.
     */
    void syncRotation();

    /**
     * Evaluates the currently active rotation and, if necessary, executes a rotation of the corresponding quadrant.
     * @pre mStartAndEnd is not empty.
     */
    void doRotation();

    /**
     * Calculates the angle between the stored LMB press start and the current mouse position.
     * @pre mStartAndEnd is not empty.
     * @return the angle in radians, in the range [-pi, +pi], where positive values indicate a clockwise rotation.
     */
    float getAngle();
};


#endif //PENTAGO_ROTATIONSTATE_H_DATE_18_08_2022_TIME_23_22
