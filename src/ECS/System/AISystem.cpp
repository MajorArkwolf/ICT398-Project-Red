#include "AISystem.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Node.hpp"
#define EPSILON 0.1f

void System::AISystem::Moving(entt::registry &ecs, double t, double dt) {
    auto entities = ecs.view<component::Moving, component::Transform>();
    for (auto &e : entities) {
        auto &move = entities.get<component::Moving>(e);
        if (!move.move_list.empty()) {
            if (ecs.has<component::Transform>(e)) {
                auto &tran = entities.get<component::Transform>(e);
                auto entity_id = move.move_list.front();
                auto moving_to = ecs.get<component::Transform>(entity_id).pos;
                auto dist = moving_to - tran.pos;
                if (!(abs(dist.x) < EPSILON && abs(dist.z) < EPSILON)) {
                    ecs.get<component::Node>(entity_id).n_o = node_occupancy::occupied;
                    glm::vec3 dir = glm::normalize(moving_to - tran.pos);
                    dir = dir * (move.speed * static_cast<float>(dt));
                    tran.pos += dir;
                    //TODO: THIS DOES NOT WORK YET, Rotation is out and needs to be addressed.
                    auto quat = glm::quat_cast(glm::lookAt(tran.pos, moving_to, {0.0f, 1.0f, 0.0f}));
                    tran.rot = quat;
                    if (ecs.has<component::Animation>(e)) {
                        auto &anim = ecs.get<component::Animation>(e);
                        auto name = anim.animator_.loaded_animation_->GetName();
                        anim.animator_.LoadAnimation("WALK", false);
                    }
                } else {
                    if (ecs.has<component::Node>(entity_id)) {
                        ecs.get<component::Node>(entity_id).n_o = node_occupancy::leaving;
                    }
                    if (ecs.has<component::Node>(move.last_node)) {
                        ecs.get<component::Node>(move.last_node).n_o = node_occupancy::vacant;
                    }
                    move.last_node = move.move_list.front();
                    move.move_list.pop();
                }
            }
        } else {
            if (ecs.has<component::Animation>(e)) {
                auto &anim = ecs.get<component::Animation>(e);
                anim.animator_.LoadAnimation("IDLE", false);
            }
        }
    }
}