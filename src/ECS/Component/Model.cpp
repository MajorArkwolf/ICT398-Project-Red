#include "Model.hpp"

#include <utility>
#include "Engine/Engine.hpp"

Component::Model::Model(const std::string &model, std::shared_ptr<Shader> newShader) {
    auto &engine = RedEngine::Engine::get();
    id = engine.modelManager.GetModelID(model);
    shader = std::move(newShader);
}
