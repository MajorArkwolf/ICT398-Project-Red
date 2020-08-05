#pragma once
#include <imgui.h>
#include <map>
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


#include "Engine/euler_camera.h"

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
    static void initialiseImGUI(GLFWwindow *window);

    /**
     * @brief Displays the input rebind menu that allows inputs to be rebound on the fly
     */
    void displayInputRebindWindow();

    /**
     * @brief Displays the escape menu
     */
    void displayEscapeMenu();

    /**
     * @brief Displays the instruction menu
     */
    void displayInstructionMenu();

    /**
     * @brief Displays the quit screen
     */
    void displayQuitScreen();

    /**
     * @brief Displays the dev screen
     */
    void displayDevScreen(engine::Camera &camera);

    /**
     * @brief Displays the texture manager
     */
    void displayTextureManager();

    /**
     * @brief Displays the terrain settings
     */
    void displayTerrainSettings();

    /**
     * @brief Starts an Imgui Window frame, must be called at the beginning of a display loop
     */
    static void startWindowFrame();

    /**
     * @brief Ends an Imgui window frame, must be called at the end of a display loop before swapping window
     */
    static void endWindowFrame();

    /**
     * @brief Toggles a window given its name in the map
     */
    void toggleWindow(const std::string &windowName);

  private:
    /// A mapping of a window name to a location in the window open array
    std::map<std::string, bool> windowOpenMap;

    /// Initialises the window open map
    void initialiseWindowOpenMap();
};
