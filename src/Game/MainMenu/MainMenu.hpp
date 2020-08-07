#pragma once
#include "Game/BaseState.hpp"
//#include "Controller/InputManager.hpp"
#include "ECS/ECS.hpp"
#include "Engine/EulerCamera.hpp"

class MainMenu : public BaseState {
public:
    /**
     * Main Menu Scene
     */
    MainMenu();

    /**
     * Destructor for main menu.
     */
    ~MainMenu() override;

    /**
     * Display method for the scene.
     */
    auto Display(const glm::mat4& projection, const glm::mat4& view) -> void override;

    /**
     * Update function for main menu.
     * @param t time since start.
     * @param dt time since last frame.
     */
    auto Update(double t, double dt) -> void override;

    /**
     * Fixed Update function for main menu.
     * @param t time since start.
     * @param dt time since last frame.
     */
    auto FixedUpdate(double t, double dt) -> void override;

    /**
     * Init function to setup any variables outside of the constructor.
     */
    void Init() override;

    /**
     * Deinits before the destructor.
     */
    void UnInit() override;

    /**
     * Input to be handled inside of the scene.
     * @param inputData events
     * @param deltaTime delta time
     */
    void HandleInputData(input::InputEvent inputData, double deltaTime) override;

    /**
     * Function to start our game.
     */
    //void startGame(model::Difficulty newDifficulty);

    /**
     * Starts the GUI
     */
    void GUIStart() override;

    /**
     * ends the GUI
     */
    void GUIEnd() override;

private:
    /**
     * Handles window events.
     */
    void HandleWindowEvent();

    /// Camera for the scene
    //Engine::Camera camera;
    void MainMenuGUI();
    bool display_difficulty_menu = false;
    ECS ecs = {};
};


