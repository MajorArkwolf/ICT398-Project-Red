#pragma once
#include <vector>
#include "DataStructures/Model/Model.hpp"
#include <array>
#include <filesystem>

class Shader;

class ModelManager {
  private:
    size_t size_ = 0;
    std::map<std::filesystem::path, size_t> name_to_id_ = {};
    std::array<model::Model, 1000> m_;
  public:
    auto ModelRepo() -> std::array<model::Model, 1000> &;
    auto GetModelID(const std::filesystem::path& filename) -> size_t;
    auto AddBasicModel(model::Model new_model) -> size_t;
    void Draw(size_t id, Shader *shader, const glm::mat4& model_matrix);
    model::Model* getModel(size_t modelID);
};
