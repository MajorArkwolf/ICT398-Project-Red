#include "Model.hpp"

#include <utility>
#include "Engine/Engine.hpp"

component::Model::Model(const std::filesystem::path &model, std::shared_ptr<Shader> new_shader) {
    auto &engine = redengine::Engine::get();
    id_ = engine.model_manager_.GetModelID(model);
    shader_ = std::move(new_shader);
}
