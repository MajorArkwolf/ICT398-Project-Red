#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include <vector>

#include "DataStructures/Model/Model.hpp"
#include "DataStructures/Model/data_types.h"
#include "DataStructures/Model/vertix.h"
#include "DrawStruct.hpp"
#include "Engine/euler_camera.h"
#include "Renderer.hpp"
#include "shader.h"
#include "Skybox.hpp"

namespace View {
    class OpenGL: public Renderer {
      public:
        /**
         * Default Constructor
         */
        OpenGL() = default;
        /**
         * Default Destructor
         */
        ~OpenGL() override;
        /**
         * Draw call for the renderer.
         */
        void Draw() override;
        /**
         * Initialises the OpenGL renderer.
         */
        void Init() override;
        /**
         * De initialises the renderer.
         */
        void DeInit() override;
        /**
         * Toggles wire frame on and off.
         */
        void ToggleWireFrame() override;
        /**
         * Setups a general mesh for the renderer in the OpenGL Context.
         * @param VAO buffer identity
         * @param VBO buffer identity
         * @param EBO buffer identity
         * @param vertices the vertices to be passed into OpenGL
         * @param indices the indices to be passed into OpenGl.
         */
        static void SetupMesh(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO,
                       std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
        /**
         * The Resize window function for OpenGL
         */
        void ResizeWindow();
        /**
         * Load a texture from file and store it.
         * @param path The path to the image.
         * @param directory The base path to ensure that no funny operating filesystem stuff happens.
         * @param gamma A flag to set if there is gamma present.
         * @return A texture ID to avoid loading duplicates.
         */
        static unsigned int TextureFromFile(const std::string& path, std::filesystem::path directory,
                                            [[maybe_unused]] bool gamma);
        /**
         * Draws a generic OpenGL Model.
         * @param shader the shader used to draw the model.
         * @param VAO index to the VAO buffer.
         * @param textures required to draw the model
         * @param indices how many indices are needed to draw the model.
         */
        static void DrawModel(Shader& shader, unsigned int &VAO, const std::vector<TextureB> &textures,
                              const std::vector<unsigned int> &indices);
        /**
         * Sets the camera to the renderer for the render pass. Required for lighting.
         * @param mainCamera the active camera in the scene.
         */
        void SetCameraOnRender(engine::Camera &mainCamera);
        /**
         * Updates the viewport.
         * @param bl Bottom left, should be 0.
         * @param br Bottom right, should be 0.
         * @param tl The width of the screen.
         * @param tr The height of the screen.
         */
        void UpdateViewPort(int bl, int br, int tl, int tr) override;

        engine::Camera* GetActiveCamera() {return camera;}

    private:
        /// Decides if the renderer should be in wire frame mode or not.
        bool wireFrame = false;
        /// The active camera on the draw pass.
        engine::Camera *camera = nullptr;
        /// The sky box for the scene.
        Skybox skyBox = {};
        /**
         * Checks to see if the window is in a minimized state.
         * @return true if minimized.
         */
        bool windowMinimized();
    };
}
