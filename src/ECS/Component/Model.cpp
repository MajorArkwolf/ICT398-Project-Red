#include "Model.hpp"
#include "Engine/Engine.hpp"

Component::Model::Model(const std::string &model) {
    auto &engine = RedEngine::Engine::get();
    id = engine.modelManager.GetModelID(model);
}
