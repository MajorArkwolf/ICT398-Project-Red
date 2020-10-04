#include "System.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Node.hpp"
#include "Engine/Engine.hpp"
#include <glm/matrix.hpp>


void System::Draw(entt::registry& registry, Shader *shader, const glm::mat4& projection, const glm::mat4& view) {
    auto entities = registry.view<component::Model, component::Transform>();
    static glm::vec4 last_color{};

    for (auto &e : entities) {
        auto &tran = entities.get<component::Transform>(e);
        auto &mod = entities.get<component::Model>(e);
        if (mod.draw_model) {
            glm::mat4 model_matrix = glm::mat4(1.0f);
            model_matrix = glm::translate(model_matrix, tran.pos);
            model_matrix = glm::scale(model_matrix, tran.scale);
            model_matrix = model_matrix * glm::mat4_cast(tran.rot);

            //shader->SetMat4("model", model_matrix);
            shader->SetBool("has_color", mod.has_color);
            if (mod.has_color && last_color != mod.color) {
                shader->SetVec4("color_value", mod.color);
                last_color = mod.color;
            }

            if (registry.has<component::Animation>(e)) {
                auto &anim = registry.get<component::Animation>(e);
                shader->SetMat4Array("jointTransforms", anim.animator_.transforms_);
                shader->SetBool("isAnimated", true);
            } else {
                shader->SetBool("isAnimated", false);
            }

            auto &model_manager = redengine::Engine::get().model_manager_;
            if (mod.wire_frame) {
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            }
            model_manager.Draw(mod.id_, shader, model_matrix);
            if (mod.wire_frame) {
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            }
        }
    }
}

void System::UpdateAnimation(entt::registry& registry, double t, double dt) {
    registry.view<component::Animation>().each ([dt](component::Animation &anim){
        anim.animator_.BoneTransform(dt);
    });
}

void System::UpdateColors(entt::registry& registry) {
    auto entities = registry.view<component::Model, component::node>();

    for (auto &e : entities) {
        auto &mod = entities.get<component::Model>(e);
        auto &node = entities.get<component::node>(e);
        if (mod.draw_model) {
            if (node.n_o == node_occupancy::vacant) {
                mod.color.r = 0.0f;
                mod.color.g = 1.0f;
                mod.color.b = 0.0f;
                mod.color.a = 1.0f;
            } else if (node.n_o == node_occupancy::occupied) {
                mod.color.r = 1.0f;
                mod.color.g = 0.0f;
                mod.color.b = 0.0f;
                mod.color.a = 1.0f;

            } else if (node.n_o == node_occupancy::leaving) {
                mod.color.r = 1.0f;
                mod.color.g = 1.0f;
                mod.color.b = 0.0f;
                mod.color.a = 1.0f;
            }
        }
    }
}
