#pragma once
#include <imgui.h>

#include <map>
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

#include "Engine/EulerCamera.hpp"

class GUIManager {
   public:
    /**
     * @brief GUIManager default constructor
     */
    GUIManager();

    /**
     * @brief Default destructor, uninitialises imgui stuff
     */
    ~GUIManager();

    /**
     * @brief Initialises ImGUI stuff
     * @param window The SDL window to draw the gui in
     */
    static void InitialiseImGUI(GLFWwindow *window);

    /**
     * @brief Displays the input rebind menu that allows inputs to be rebound on the fly
     */
    void DisplayInputRebindWindow();

    /**
     * @brief Displays the escape menu
     */
    void DisplayEscapeMenu();

    /**
     * @brief Displays the dev screen
     */
    void DisplayDevScreen(engine::Camera &camera);

    /**
     * @brief Displays the log console
     */
    void DisplayConsoleLog();

    /**
     * @brief Starts an Imgui Window frame, must be called at the beginning of a display loop
     */
    static void startWindowFrame();

    /**
     * @brief Ends an Imgui window frame, must be called at the end of a display loop before swapping window
     */
    static void EndWindowFrame();

    void DisplayQuitScreen();

    /**
     * @brief Toggles a window given its name in the map
     */
    void ToggleWindow(const std::string &windowName);

    /**
     * Displays the AI Menu
     * @param entity entity your viewing.
     * @param registry the registry being used.
     */
    void DisplayAI(entt::entity &entity, entt::registry& registry);

    bool renderer_ = false;

   private:
    /// A mapping of a window name to a location in the window open array
    std::map<std::string, bool> window_open_map;

    /// Initialises the window open map
    void InitialiseWindowOpenMap();

    unsigned int exit_texture_id = 0;
};
