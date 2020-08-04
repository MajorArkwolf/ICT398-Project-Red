#include "System.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Basic.hpp"
#include "Engine/Engine.hpp"
#include <glm/matrix.hpp>


void System::Draw(entt::registry& registry, const glm::mat4& projection, const glm::mat4& view) {
    auto entities = registry.view<component::Model, component::Transform>();

    for (auto &e : entities) {
        auto &tran = entities.get<component::Transform>(e);
        auto &mod = entities.get<component::Model>(e);
        glm::mat4 model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, tran.pos);
        model_matrix = glm::scale(model_matrix, tran.scale);
        model_matrix = model_matrix * glm::mat4_cast(tran.rot);

        mod.shader_->use();
        mod.shader_->setMat4("projection", projection);
        mod.shader_->setMat4("view", view);
        mod.shader_->setMat4("model", model_matrix);

        if (registry.has<component::Animation>(e)) {
            auto &anim = registry.get<component::Animation>(e);
            mod.shader_->setMat4Array("jointTransforms", anim.animator_.Transforms);
            mod.shader_->setBool("isAnimated", true);
        } else {
            mod.shader_->setBool("isAnimated", false);
        }

        auto& model_manager = RedEngine::Engine::get().modelManager;
        model_manager.Draw(mod.id_, mod.shader_.get());
    }
}

void System::UpdateAnimation(entt::registry& registry, double t, double dt) {
    registry.view<component::Animation>().each ([dt](component::Animation &anim){
        anim.animator_.BoneTransform(dt);
    });
}
