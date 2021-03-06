#pragma once
#include <string>
#include <unordered_map>
#include "Engine/Renderer/Shader.hpp"
#include <memory>
#include <optional>

class ShaderRepo {
public:
    ShaderRepo() = default;
    ~ShaderRepo() = default;
    ShaderRepo(ShaderRepo& shaderRepo) = delete;
    ShaderRepo(ShaderRepo&& shaderRepo) = default;
    ShaderRepo& operator=(ShaderRepo& shaderRepo) = delete;
    ShaderRepo& operator=(ShaderRepo&& shaderRepo) = default;
    std::shared_ptr<Shader> ImportShader(const std::filesystem::path& filepath);
    std::optional<std::shared_ptr<Shader>> GetShader(const std::filesystem::path& filepath);
private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> prefabMap_ = {};
};

