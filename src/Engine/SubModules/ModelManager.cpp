#include "ModelManager.hpp"

#include <map>

auto ModelManager::GetModelID(const std::filesystem::path& filename) -> size_t {
    auto id = name_to_id.find(filename);
    if (id == name_to_id.end()) { // file not loaded yet
        m[size] = model::Model(filename, false);
        name_to_id.emplace(filename, size);
        ++size;
        return size - 1;
    } else {
        return id->second;
    }
}

void ModelManager::Draw(size_t id, Shader *ourShader) {
    ModelRepo().at(id).Draw(*ourShader);
}

auto ModelManager::ModelRepo() -> std::array<model::Model, 1000> & {
    return m;
}
model::Model *ModelManager::getModel(size_t modelID) {
    return &m[modelID];
}
