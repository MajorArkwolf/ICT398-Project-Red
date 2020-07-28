#include <functional>
#include <Engine/Renderer/DrawStruct.hpp>
//#include <Controller/PhysicsManager.hpp>
#include <Engine/Engine.hpp>
#include <glm/gtx/quaternion.hpp>
#include "StaticModel.hpp"
#include "Engine/Engine.hpp"

MainMenuObject::StaticModel::StaticModel(const std::string& name) {
    auto &rm = RedEngine::Engine::get().modelManager;
    model = rm.GetModelID(name);
    auto basepath = RedEngine::Engine::get().getBasePath();
    auto vert = basepath / "res" / "shader" / "vertshader.vs";
    auto frag = basepath / "res" / "shader" / "fragshader.fs";
    shader = std::make_shared<Shader>(Shader(vert, frag, std::filesystem::path{}));
}

MainMenuObject::StaticModel::~StaticModel() = default;

void MainMenuObject::StaticModel::Update(double t, double dt) {
    if (animator != nullptr) {
        animator->BoneTransform(dt);
    }
}

void MainMenuObject::StaticModel::Draw(const glm::mat4 &projection, const glm::mat4 &view) {
    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = modelMatrix * glm::toMat4(rotation);
    shader->setMat4("model", modelMatrix);
    if (animator) {
        shader->setMat4Array("jointTransforms", animator->Transforms);
        shader->setBool("isAnimated", true);
    } else {
        shader->setBool("isAnimated", false);
    }
    // Get the resource manager and call for it to draw the model
    //auto& res_manager = ResourceManager::getInstance();
    auto &rm = RedEngine::Engine::get().modelManager;
    rm.Draw(model, shader.get());
    //res_manager.drawModel(model, shader.get());
}

size_t MainMenuObject::StaticModel::getModel() const {
    return model;
}
