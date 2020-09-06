#include "ModelManager.hpp"

#include <map>

auto ModelManager::GetModelID(const std::filesystem::path& filename) -> size_t {
    auto id = name_to_id_.find(filename);
    if (id == name_to_id_.end()) { // file not loaded yet
        m_[size_] = model::Model(filename, false);
        name_to_id_.emplace(filename, size_);
        ++size_;
        return size_ - 1;
    } else {
        return id->second;
    }
}

void ModelManager::Draw(size_t id, Shader *shader) {
    ModelRepo().at(id).Draw(shader);
}

auto ModelManager::ModelRepo() -> std::array<model::Model, 1000> & {
    return m_;
}
model::Model *ModelManager::getModel(size_t modelID) {
    return &m_[modelID];
}
