#include "Model.hpp"

#include <utility>
#include "Engine/Engine.hpp"

component::Model::Model(const std::filesystem::path &model) {
    auto &engine = redengine::Engine::get();
    id_ = engine.model_manager_.GetModelID(model);
}

component::Model::Model(size_t model_id) {
    id_ = model_id;
}
