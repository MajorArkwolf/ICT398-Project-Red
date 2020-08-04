#include "Model.hpp"

#include <utility>
#include "Engine/Engine.hpp"

component::Model::Model(const std::filesystem::path &model, std::shared_ptr<Shader> new_shader) {
    auto &engine = RedEngine::Engine::get();
    id_ = engine.modelManager.GetModelID(model);
    shader_ = std::move(new_shader);
}
