#include "ShaderRepo.hpp"
#include "Engine/Engine.hpp"

std::shared_ptr<Shader> ShaderRepo::ImportShader(const std::filesystem::path &filepath) {
    if (prefabMap_.find(filepath.c_str()) == prefabMap_.end()) {
        auto base_path = redengine::Engine::get().GetBasePath();
        auto vertix_file = base_path / filepath / ".vs";
        auto fragment_file = base_path / filepath / ".fs";
        auto shader = std::make_shared<Shader>(vertix_file, fragment_file, "");
        prefabMap_.emplace(filepath.c_str(), shader);
        return shader;
    } else {
        return prefabMap_.at(filepath.c_str());
    }
}

std::optional<std::shared_ptr<Shader>> ShaderRepo::GetShader(const std::filesystem::path &filepath) {
    auto shader = std::optional<std::shared_ptr<Shader>>();
    if (prefabMap_.find(filepath.c_str()) != prefabMap_.end()) {
        shader = prefabMap_.at(filepath.c_str());
    } else {
        assert(0);
    }
    return shader;
}
