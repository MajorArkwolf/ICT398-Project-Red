#pragma once

#include "Controller/InputManager.hpp"
#include "View/EulerCamera.hpp"

/**
 * @class BaseState
 * @brief Pure virtual class for a game state
 */
class BaseState {
  public:
    /**
     * @brief Virtual destructor for BaseState
     */
    virtual ~BaseState() = default;

    /// Whether mouse is set to relative mode (reset to center of window each frame)
    bool relativeMouse = 1;

    /**
     * @brief Virtual initialisation function for a game state
     * Used separately from a hard init to reset variables needed for game state function
     */
    virtual void Init() = 0;

    /**
     * @brief Virtual soft initialisation function for a game state
     * Used separately from a hard init to reset variables needed for games tate function
     */
    virtual void display() = 0;

    /**
     * @brief Virtual un-initialisation function for a game state
     * Used to free memory from the game state, called right before popping the game stack
     */
    virtual void unInit() = 0;

    /**
     * @brief Virtual function for handling input events encased within a a custom Input Event
     * All input events will be sent to the current game state at the top of the stack
     * @param inputData The InputData event that contains the user input
     * @param deltaTime time since last frame
     */
    virtual void handleInputData(Controller::Input::InputData inputData, double deltaTime) = 0;

    /**
     * @brief Virtual update function for updating physics within a game state
     * @param dt Delta time since last frame
     * @param t The time since the program was started
     */
    virtual void update(double t, double dt) = 0;

    /// Camera required for renderer.
    View::Camera camera;

    /**
     * Starts the GUI menu.
     */
    virtual void GUIStart() = 0;

    /**
     * Ends the GUI menu.
     */
    virtual void GUIEnd() = 0;

};
