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
        Model(const std::filesystem::path &model, std::shared_ptr<Shader> new_shader);
        size_t id_ = 0;
        std::shared_ptr<Shader> shader_ = nullptr;
    };
}
