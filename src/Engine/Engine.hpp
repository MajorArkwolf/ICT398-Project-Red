#pragma once

#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLEQ_IMPLEMENTATION
#define GLEQ_STATIC
//#include "gleq.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Game/BaseState.hpp"
#include "Engine/GameStack.hpp"
#include "Engine/Renderer/OpenGL.hpp"
#include "Engine/SubModules/ModelManager.hpp"

namespace RedEngine {

    /**
     * @class Engine
     * @brief The main game engine
     */
    class Engine {
      public:
        static constexpr auto FPS_UPDATE_INTERVAL = 0.5;

        /// Mouse movement.
        glm::vec2 mouse = {};

        /// GLFW handles.
        GLFWwindow *window = nullptr;

        /// Renderer for OpenGL
        View::OpenGL renderer = {};

        /// The current FPS
        double fps           = 0.0;
        /// Base path to the program.
        std::string basepath = "";

        /// The game stack to allow to switch between scenes.
        ///
        GameStack<std::shared_ptr<BaseState>> gameStack;
      private:


        /// GUI Manager for our GUI interface.
        //GUIManager guiManager;
        /// Flag used to determine if the engine should shutdown.
        bool isRunning = true;
        /**
         * Gets the basepath of where the engine is running.
         */
        auto getBasePath() -> void;
        /**
         * Privatised constructor due to the engine being a singleton.
         */
        Engine();

        /// Testing values
        int lastWindowXSize     = 800;
        int lastWindowYSize     = 600;
        bool firstMouse = true;

        double t  = 0.0;
        double dt = 0.01;
        double EngineFrameTime   = 0.0;
        std::string glsl_version = "";


      public:
        /**
         * Deleted move constructor due to unique pointers being used.
         */
        Engine(Engine &&)      = delete;

        /**
         * Deleted move destructor due to unique pointers being used.
         */
        Engine(const Engine &) = delete;

        /**
         * Destructor for the engine
         */
        ~Engine();

        /**
         * @brief Getter to the engine variables.
         * @return Engine by reference
         */
        static auto get() -> Engine &;

        /**
         * @brief The game engine main loop
         */
        static auto run() -> void;

        /**
         * Gets the GUI manager interface.
         * @return the GUIManager object.
         */
        //GUIManager &getGuiManager();

        /**
         * @brief Overloaded assignment operator, set to default overload
         */
        auto operator=(Engine &&) -> Engine & = delete;

        /**
         * @brief Overloaded const assignment operator, set to delete overload
         */
        auto operator=(const Engine &) -> Engine & = delete;

        /**
         * Check to see if the engine is running.
         * @return the current state of the engine.
         */
        auto getIsRunning() const -> bool;

        /**
         * Tells the engine the program is requesting termination.
         */
        auto endEngine() -> void;

        /**
         * Process the input from our 3rd party library.
         */
        void processInput(double deltaTime);

        /**
         * Checks to see what type of mouse mode the engine has set.
         * @return the value of the mouse.
         */
        bool getMouseMode();

        /**
         * Sets the mouse mode
         * @param mode sets the mouse mode.
         */
        void setMouseMode(bool mode);


        void SettingMenu();

        ///Engine Sub Modules
        ModelManager modelManager = {};

        ///Engine Variables

        float gammaCorrection = 1.f;
        bool showSettingsMenu = false;

        int getLastWindowXSize() const;

        void setLastWindowXSize(int lastWindowXSize);

        int getLastWindowYSize() const;

        void setLastWindowYSize(int lastWindowYSize);

        double getT() const;
        double getDt() const;
        double getFrameTime() const;
    };
}
