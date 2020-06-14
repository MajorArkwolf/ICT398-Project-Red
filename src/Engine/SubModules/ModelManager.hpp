#pragma once
#include <vector>
#include "DataStructures/Model/Model.hpp"
#include <array>

class Shader;

class ModelManager {
  private:
    size_t size = 0;
    std::map<std::string, size_t> nameToId = {};
    std::array<Model::Model, 1000> m;
  public:
    auto ModelRepo() -> std::array<Model::Model, 1000> &;
    auto GetModelID(const std::string& filename) -> size_t;
    void Draw(size_t id, Shader *ourShader);
    Model::Model* getModel(size_t modelID);
};
