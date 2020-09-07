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
#include "Engine/SubModules/Input/InputManager.hpp"
#include "Engine/SubModules/PrefabRepo.hpp"
#include "Engine/SubModules/ShaderRepo.hpp"
#include "SubModules/ConsoleLog.hpp"

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
        PrefabRepo prefabRepo_ = {};
        ShaderRepo shaderRepo_ = {};
        std::string glsl_version_ = "";
        /// Base path to the program.
        std::filesystem::path base_path_ = {};
        ConsoleLog log_;
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

        /**
         * Gets the last known size of a window.
         * @return Gets the last known size of the given window.
         */
        int GetLastWindowXSize() const;

        /**
         * Set the Screen X size.
         * @param last_window_x_size set the X size of the window.
         */
        void SetLastWindowXSize(int last_window_x_size);

        /**
         * Gets the last known size of a window.
         * @return Gets the last known size of the given window.
         */
        int GetLastWindowYSize() const;

        /**
         * Set the Screen Y size.
         * @param last_window_y_size set the Y size of the window.
         */
        void SetLastWindowYSize(int last_window_y_size);

        /**
         * Get the time since the engine executed.
         * @return the time since engine loop began.
         */
        double GetT() const;

        /**
         * Returns the Delta time, this is a fixed time step so you should expect consistent values.
         * @return Delta time, fixed time step.
         */
        double GetDt() const;

        /**
         * Returns the delta time for a given frame.
         * @return Delta time relative to the frame.
         */
        double GetFrameTime() const;

        /**
         * Gets the basepath of the executable
         */
        input::InputManager input_manager_;

        /**
         * Get the basepath relative to the executable.
         * @return file address
         */
        auto GetBasePath() const -> std::filesystem::path;
        /**
         * Gets the prefab repo from the engine.
         * @return a reference to the prefab repo.
         */
        PrefabRepo& GetPrefabRepo();

        ConsoleLog &GetLog();

        void CloseScene();
    };
}
