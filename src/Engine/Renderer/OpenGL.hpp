#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Renderer.hpp"
#include "Shader.hpp"
#include "DrawStruct.hpp"
#include "Model/Models/DataTypes.hpp"
#include "Skybox.hpp"
#include "View/EulerCamera.hpp"
#include "Model/Models/Model.hpp"
#include "Model/Vertix.hpp"

namespace View {
    class OpenGL: public View::Renderer {
      public:
        /**
         * Default Constructor
         */
        OpenGL() = default;
        /**
         * Default Destructor
         */
        ~OpenGL() = default;
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
         * Adds an item to the draw que.
         * @param drawItem Passes DrawItem into the que to be used for rendering purposes.
         */
        void AddToQue(View::Data::DrawItem& drawItem);
        /**
         * Adds an item to the draw que.
         * @param drawItem Passes DrawItem into the que to be used for rendering purposes.
         */
        void AddToQueTransparent(View::Data::DrawItem& drawItem);
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
        static unsigned int TextureFromFile(const char *path, const std::string &directory,
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
        void SetCameraOnRender(Camera &mainCamera);
        /**
         * Setup the terrain models
         * @param VAO index of buffer.
         * @param VBO index of buffer.
         * @param EBO index of buffer.
         * @param vertices to load into the buffers.
         * @param indices to load into the buffers.
         */
        void SetupTerrainModel(unsigned int &VAO, unsigned &VBO, unsigned int &EBO, const std::vector<Blue::Vertex>& vertices, const std::vector<unsigned int>& indices);

        /**
         * Draw the Terrain models
         * @param VAO index to the buffer object in OpenGL.
         * @param textures a vector of texture ID's used to generate the terrain.
         * @param ebo_size The size of the indices buffer.
         */
        void DrawTerrain(unsigned int &VAO, const std::vector<unsigned int> &textures,
                         const unsigned int& ebo_size);

        /**
         * Updates the viewport.
         * @param bl Bottom left, should be 0.
         * @param br Bottom right, should be 0.
         * @param tl The width of the screen.
         * @param tr The height of the screen.
         */
        void UpdateViewPort(int bl, int br, int tl, int tr) override;

    private:
        /// Decides if the renderer should be in wire frame mode or not.
        bool wireFrame = false;
        /// DrawQue of objects that need to be rendered by the renderer.
        std::vector<View::Data::DrawItem> drawQue = {};
        /// A draw que only for transparent objects.
        std::vector<View::Data::DrawItem> drawQueTransparent = {};
        /// The active camera on the draw pass.
        Camera *camera = nullptr;
        /// The sky box for the scene.
        Skybox skyBox = {};

        /**
         * Sorts the objects based on the view distance, helps solve alpha issues.
         */
        void sortDrawDistance();

        /**
         * Checks to see if the window is in a minimized state.
         * @return true if minimized.
         */
        bool windowMinimized();
    };
}
