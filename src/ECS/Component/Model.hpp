#pragma once
#include <string>
#include "Engine/Renderer/Shader.hpp"

namespace Component {
    struct Model {
        Model(const std::string &model, std::shared_ptr<Shader> newShader);
        size_t id = 0;
        std::shared_ptr<Shader> shader = nullptr;
    };
}
