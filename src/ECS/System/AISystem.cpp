#include "AISystem.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Node.hpp"
#include "ECS/Component/Pathing/Node.hpp"
#define EPSILON 0.1f

static inline void AlterNodeGrid(Pathing::Node * node, bool occupied) {
    if (node != nullptr) {
        node->occupied = occupied;
    }
}

void System::AISystem::Moving(entt::registry &ecs, double t, double dt) {
    auto entities = ecs.view<component::Moving, component::Transform>();
    // Iterate over all the components that have a moving and a transform component
    for (auto &e : entities) {
        // We store the moving component then check if the there is a que.
        auto &move = entities.get<component::Moving>(e);
        if (!move.move_list.empty()) {
            if (ecs.has<component::Transform>(e)) {
                auto &tran = entities.get<component::Transform>(e);
                auto entity_id = move.move_list.front();
                auto moving_to = ecs.get<component::Transform>(entity_id).pos;
                auto dist = moving_to - tran.pos;
                // We check to see if the object is within a certain epsilion before determining it is at a given node.
                if (!(abs(dist.x) < EPSILON && abs(dist.z) < EPSILON)) {
                    auto &node_comp = ecs.get<component::Node>(entity_id);
                    node_comp.n_o = node_occupancy::occupied;
                    AlterNodeGrid(node_comp.grid_node, true);
                    glm::vec3 dir = glm::normalize(moving_to - tran.pos);
                    dir = dir * (move.speed * static_cast<float>(dt));
                    tran.pos += dir;
                    tran.rot = glm::quatLookAt(glm::normalize(tran.pos - moving_to), {0.0f, 1.0f, 0.0f});
                    if (ecs.has<component::Animation>(e)) {
                        auto &anim = ecs.get<component::Animation>(e);
                        anim.animator_.LoadAnimation("WALK", false);
                    }
                } else {
                    auto &node_comp = ecs.get<component::Node>(entity_id);
                    if (ecs.has<component::Node>(entity_id)) {
                        ecs.get<component::Node>(entity_id).n_o = node_occupancy::leaving;
                    }
                    if (ecs.has<component::Node>(move.last_node)) {
                        AlterNodeGrid(node_comp.grid_node, false);
                        ecs.get<component::Node>(move.last_node).n_o = node_occupancy::vacant;
                    }
                    move.last_node = move.move_list.front();
                    move.move_list.pop();
                }
            }
        } else {
            if (ecs.has<component::Animation>(e)) {
                auto &anim = ecs.get<component::Animation>(e);
                if (anim.animator_.loaded_animation_ != nullptr) {
                    if (anim.animator_.loaded_animation_->GetName() == "WALK") {
                        anim.animator_.LoadAnimation("IDLE", false);
                    }
                }
            }
        }
    }
}
