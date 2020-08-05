#pragma once

#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <filesystem>
#include "Game/BaseState.hpp"
#include "Engine/GameStack.hpp"
#include "Engine/Renderer/OpenGL.hpp"
#include "Engine/SubModules/ModelManager.hpp"
#include "Engine/SubModules/GUIManager.hpp"

namespace redengine {

    /**
     * @class Engine
     * @brief The main game engine
     */
    class Engine {
      public:
        /// Mouse movement.
        glm::vec2 mouse_ = {};

        /// GLFW handles.
        GLFWwindow *window_ = nullptr;

        /// Renderer for OpenGL
        view::OpenGL renderer_ = {};

        /// The game stack to allow to switch between scenes.
        ///
        GameStack<std::shared_ptr<BaseState>> game_stack_;
      private:
        /// GUI Manager for our GUI interface.
        GUIManager gui_manager_;
        /// Flag used to determine if the engine should shutdown.
        bool is_running_ = true;
        /**
         * Sets the basepath of where the engine is running.
         */
        auto SetupBasePath() -> void;
        /**
         * Privatised constructor due to the engine being a singleton.
         */
        Engine();

        /// Testing values
        int last_window_x_size_     = 800;
        int last_window_y_size_     = 600;

        double t_  = 0.0;
        double dt_ = 0.01;
        double engine_frame_time_   = 0.0;
        std::string glsl_version_ = "";
        /// Base path to the program.
        std::filesystem::path base_path_ = {};
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
        static auto Run() -> void;

        /**
         * Gets the GUI manager interface.
         * @return the GUIManager object.
         */
        GUIManager &GetGuiManager();

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
        auto GetIsRunning() const -> bool;

        /**
         * Tells the engine the program is requesting termination.
         */
        auto EndEngine() -> void;

        /**
         * Process the input from our 3rd party library.
         */
        void ProcessInput(double deltaTime);

        /**
         * Checks to see what type of mouse mode the engine has set.
         * @return the value of the mouse.
         */
        bool GetMouseMode();

        /**
         * Sets the mouse mode
         * @param mode sets the mouse mode.
         */
        void GetMouseMode(bool mode);


        void SettingMenu();

        ///Engine Sub Modules
        ModelManager model_manager_ = {};

        ///Engine Variables

        float gamma_correction_ = 1.f;
        bool show_settings_menu_ = false;

        int GetLastWindowXSize() const;

        void SetLastWindowXSize(int last_window_x_size);

        int GetLastWindowYSize() const;

        void SetLastWindowYSize(int last_window_y_size);

        double GetT() const;
        double GetDt() const;
        double GetFrameTime() const;

        /**
         * Gets the basepath of the executable
         */
        auto GetBasePath() const -> std::filesystem::path;
    };
}
