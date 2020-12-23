#pragma once
#include <string>
#include <filesystem>
#include "Engine/Renderer/Shader.hpp"

namespace component {
    struct Model {
        /**
         * Generate model component
         * @param model the filepath to the model relative to the exe.
         * @param new_shader the shader to be used for the model.
         */
        explicit Model(const std::filesystem::path &model);
        explicit Model(size_t model_id);
        size_t id_ = 0;
        bool draw_model = true;
        bool wire_frame = false;
        bool has_color = false;
        glm::vec4 color = {};
    };
}
