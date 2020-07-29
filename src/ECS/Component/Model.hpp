#pragma once
#include <string>
#include <filesystem>
#include "Engine/Renderer/Shader.hpp"

namespace Component {
    struct Model {
        Model(const std::filesystem::path &model, std::shared_ptr<Shader> newShader);
        size_t id = 0;
        std::shared_ptr<Shader> shader = nullptr;
    };
}
