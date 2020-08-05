#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <filesystem>
#include "Engine/Renderer/Shader.hpp"
#include "DataStructures/Model/Animator.hpp"

namespace MainMenuObject {
    /**
     * This class is only used for demonstration purposes, documentation will not be provided as it should only
     * be used for demonstration purposes only.
     */
    class StaticModel {
    public:
        explicit StaticModel(const std::filesystem::path& name);
        ~StaticModel() = default;
        void Update(double t, double dt);
        void Draw(const glm::mat4& projection, const glm::mat4& view);
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
        std::shared_ptr<Model::Animator> animator = nullptr;
    private:
        size_t model = 0;
        std::shared_ptr<Shader> shader = nullptr;
    public:
        size_t getModel() const;
    };
}

