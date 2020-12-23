#include "ShaderRepo.hpp"
#include "Engine/Engine.hpp"

std::shared_ptr<Shader> ShaderRepo::ImportShader(const std::filesystem::path &filepath) {
    if (prefabMap_.find(filepath.generic_string()) == prefabMap_.end()) {
        auto base_path = redengine::Engine::get().GetBasePath();
        auto vertix_file = base_path / filepath;
        vertix_file.replace_extension(".vs");
        auto fragment_file = base_path / filepath;
        fragment_file.replace_extension(".fs");
        auto shader = std::make_shared<Shader>(vertix_file, fragment_file, "");
        prefabMap_.emplace(filepath.generic_string(), shader);
        return prefabMap_.at(filepath.generic_string());
    } else {
        return prefabMap_.at(filepath.generic_string());
    }
}

std::optional<std::shared_ptr<Shader>> ShaderRepo::GetShader(const std::filesystem::path &filepath) {
    auto shader = std::optional<std::shared_ptr<Shader>>();
    if (prefabMap_.find(filepath.generic_string()) != prefabMap_.end()) {
        shader = prefabMap_.at(filepath.generic_string());
    } else {
        assert(0);
    }
    return shader;
}
