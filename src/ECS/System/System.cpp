#include "System.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Basic.hpp"
#include "Engine/Engine.hpp"
#include <glm/matrix.hpp>


void System::Draw(entt::registry& registry, const glm::mat4& projection, const glm::mat4& view) {
    auto entities = registry.view<Component::Model, Component::Transform>();

    for (auto &e : entities) {
        auto &tran = entities.get<Component::Transform>(e);
        auto &mod = entities.get<Component::Model>(e);
        glm::mat4 model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, tran.pos);
        model_matrix = glm::scale(model_matrix, tran.scale);
        model_matrix = model_matrix * glm::mat4_cast(tran.rot);

        mod.shader->use();
        mod.shader->setMat4("projection", projection);
        mod.shader->setMat4("view", view);
        mod.shader->setMat4("model", model_matrix);

        if (registry.has<Component::Animation>(e)) {
            auto &anim = registry.get<Component::Animation>(e);
            mod.shader->setMat4Array("jointTransforms", anim.animator.Transforms);
            mod.shader->setBool("isAnimated", true);
        } else {
            mod.shader->setBool("isAnimated", false);
        }

        auto& model_manager = RedEngine::Engine::get().modelManager;
        model_manager.Draw(mod.id, mod.shader.get());
    }
}

void System::UpdateAnimation(entt::registry& registry, double t, double dt) {
    registry.view<Component::Animation>().each ([dt](Component::Animation &anim){
        anim.animator.BoneTransform(dt);
    });
}
