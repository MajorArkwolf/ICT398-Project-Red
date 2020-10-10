#include "System.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Basic.hpp"
#include "Engine/Engine.hpp"
#include <glm/matrix.hpp>


void System::Draw(entt::registry& registry, Shader *shader, const glm::mat4& projection, const glm::mat4& view) {
    auto entities = registry.view<component::Model, component::Transform>();

    for (auto &e : entities) {
        auto &tran = entities.get<component::Transform>(e);
        auto &mod = entities.get<component::Model>(e);
        glm::mat4 model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, static_cast<glm::vec3>(tran.pos));
        model_matrix = glm::scale(model_matrix, glm::vec3(tran.scale));
        model_matrix = model_matrix * glm::mat4_cast(static_cast<glm::quat>(tran.rot));

        shader->SetMat4("model", model_matrix);

        if (registry.has<component::Animation>(e)) {
            auto &anim = registry.get<component::Animation>(e);
            shader->SetMat4Array("jointTransforms", anim.animator_.transforms_);
            shader->SetBool("isAnimated", true);
        } else {
            shader->SetBool("isAnimated", false);
        }

        auto& model_manager = redengine::Engine::get().model_manager_;
        model_manager.Draw(mod.id_, shader);
    }
}

void System::UpdateAnimation(entt::registry& registry, double t, double dt) {
    registry.view<component::Animation>().each ([dt](component::Animation &anim){
        anim.animator_.BoneTransform(dt);
    });
}
