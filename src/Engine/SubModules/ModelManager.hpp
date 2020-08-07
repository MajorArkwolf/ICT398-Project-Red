#pragma once
#include <vector>
#include "DataStructures/Model/Model.hpp"
#include <array>
#include <filesystem>

class Shader;

class ModelManager {
  private:
    size_t size = 0;
    std::map<std::filesystem::path, size_t> nameToId = {};
    std::array<model::Model, 1000> m;
  public:
    auto ModelRepo() -> std::array<model::Model, 1000> &;
    auto GetModelID(const std::filesystem::path& filename) -> size_t;
    void Draw(size_t id, Shader *ourShader);
    model::Model* getModel(size_t modelID);
};
