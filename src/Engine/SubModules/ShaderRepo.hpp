#pragma once
#include <string>
#include <unordered_map>
#include "Engine/Renderer/Shader.hpp"
#include <memory>

class ShaderRepo {
public:
    ShaderRepo() = default;
    ~ShaderRepo() = default;
    ShaderRepo(ShaderRepo& shaderRepo) = delete;
    ShaderRepo(ShaderRepo&& shaderRepo) = default;
    ShaderRepo& operator=(ShaderRepo& shaderRepo) = delete;
    ShaderRepo& operator=(ShaderRepo&& shaderRepo) = default;
private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> prefabMap_ = {};
};

