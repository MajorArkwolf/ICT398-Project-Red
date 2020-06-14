#include <functional>
#include <Engine/Renderer/DrawStruct.hpp>
//#include <Controller/PhysicsManager.hpp>
#include <Engine/Engine.hpp>
#include <glm/gtx/quaternion.hpp>
#include "StaticModel.hpp"

MainMenuObject::StaticModel::StaticModel(const std::string& name) {
    auto &rm = RedEngine::Engine::get().modelManager;
    model = rm.getModelID(name);
    shader = std::make_shared<Shader>(Shader("res/shader/vertshader.vs", "res/shader/fragshader.fs"));
}

MainMenuObject::StaticModel::~StaticModel() {

}

void MainMenuObject::StaticModel::update(double t, double dt) {
    if (animator != nullptr) {
        animator->BoneTransform(dt);
    }
}

void MainMenuObject::StaticModel::addToDraw() {
    // Create a function pointer of the GameObject's draw call for the DrawItem
    std::function<void(const glm::mat4& projection, const glm::mat4& view, const glm::dvec3& cameraPos)> e = [&](const glm::mat4& projection, const glm::mat4& view, const glm::dvec3& cameraPos) {
        this->draw(projection, view, cameraPos);
    };

    // Here the DrawItem for the GameObject is generated and configured
    View::Data::DrawItem drawItem = {};
    drawItem.drawPointer = e;

    // Finally the DrawItem is added to the renderer queue
    auto& renderer = RedEngine::Engine::get().renderer;
    renderer.AddToQue(drawItem);
}

void MainMenuObject::StaticModel::draw(const glm::mat4 &projection, const glm::mat4 &view, const glm::dvec3 &cameraPos) {
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
    rm.Draw(model, shader.get())
    //res_manager.drawModel(model, shader.get());
}

size_t MainMenuObject::StaticModel::getModel() const {
    return model;
}
