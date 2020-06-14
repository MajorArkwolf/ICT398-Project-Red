#pragma once

#include "View/Renderer/Shader.hpp"
#include <vector>
#include <string>
#include <memory>

using std::string, std::vector;
namespace View {
    /**
     * Basic OpenGL Sky box.
     */
    class Skybox {
      public:
        /**
         * Constructor for skybox.
         */
        Skybox() = default;
        /**
         * Destructor for Sky box.
         */
        ~Skybox();
        /**
         * Deleted copy constructor.
         */
        Skybox(const View::Skybox &) = delete;
        /**
         * Deleted move constructor.
         */
        Skybox(View::Skybox &&) = delete;
        /**
         * Deleted copy assignment.
         * @return
         */
        Skybox& operator=(const View::Skybox &) = delete;
        /**
         * Deleted move assignment.
         * @return
         */
        Skybox&& operator=(View::Skybox &&) = delete;
        /**
         * Initialises the Sky box.
         */
        void Init();
        /**
         * Draw function for the sky box. Should be called near the end of a draw call.
         * @param view matrix from the camera.
         * @param projection matrix from the camera.
         */
        void draw(const glm::mat4& view, const glm::mat4& projection) const;
        /**
         * Update doesnt do anything, however in the future it may.
         */
        void update();
        /// Unique pointer to the skybox shader.
        std::unique_ptr<Shader> shader = nullptr;

      private:
        /// Buffer objects for opengl.
        unsigned int skyboxVAO = 0, skyboxVBO = 0;
        /// Texture assigned to the skybox.
        unsigned int cubemapTexture = 0;
        /// Faces used by the skybox to generate the cube map.
        vector<string> faces = {};
        /// The coordinates to form a cube.
        float skyboxVertices[108] = {
            // positions
            -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
            1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

            -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
            -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

            1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

            -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
            1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

        /**
         * Load the cube map into the renderer.
         * @param faces the faces to be loaded in.
         * @return the buffer object id.
         */
        unsigned int loadCubemap(vector<string> faces);


    };
}
