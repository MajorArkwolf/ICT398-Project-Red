#include "AISystem.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Node.hpp"
#include "ECS/Component/Pathing/Node.hpp"
#include "ECS/Component/Board.hpp"
#include <random>
#define EPSILON 0.1f

#include <chrono>
#include <random>
static inline size_t GenerateRandom(size_t min, size_t max) {
    size_t returnValue = 0;
    std::random_device rd;
    std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
                    std::chrono::duration_cast<std::chrono::seconds>(
                            std::chrono::system_clock::now().time_since_epoch()
                    ).count() +
            (std::mt19937::result_type)
                    std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::high_resolution_clock::now().time_since_epoch()
                    ).count() );

    std::mt19937 gen(seed);
    std::uniform_int_distribution<size_t> distrib(min, max);
    returnValue = distrib(gen);
    return returnValue;
}

void System::AISystem::Moving(entt::registry &ecs, double t, double dt) {
    auto entities = ecs.view<component::Moving, component::Transform>();
    // Iterate over all the components that have a moving and a transform component
    for (auto &e : entities) {
        // We store the moving component then check if the there is a que.
        auto &move = entities.get<component::Moving>(e);
        if (!move.move_list.empty() && move.is_moving) {
            auto &next_node = ecs.get<component::Node>(move.move_list.front());
            if (next_node.GetNodeStatus() != node_occupancy::vacant) {
                auto board_id = ecs.view<component::Board>()[0];
                auto &board = ecs.get<component::Board>(board_id);
                ecs.get<component::Node>(move.going_to_node).AlterNodeGrid(node_occupancy::vacant);
                ecs.get<component::Node>(move.last_node).AlterNodeGrid(node_occupancy::vacant);
                move.move_list = board.FindPath(ecs, move.move_list.front(), move.last_node);
                if (move.move_list.empty()) {
                    continue;
                }
                move.going_to_node = move.move_list.front();
                move.move_list.pop();
            }
            if (ecs.has<component::Transform>(e)) {
                auto &tran = entities.get<component::Transform>(e);
                auto entity_id = move.going_to_node;
                if (!ecs.has<component::Transform>(entity_id)) {
                    continue;
                }
                auto moving_to = ecs.get<component::Transform>(entity_id).pos;
                auto dist = moving_to - tran.pos;
                // We check to see if the object is within a certain epsilion before determining it is at a given node.
                if (!(abs(dist.x) < EPSILON && abs(dist.z) < EPSILON)) {
                    auto &node_comp = ecs.get<component::Node>(entity_id);
                    node_comp.AlterNodeGrid(node_occupancy::occupied);
                    auto first = tran.pos;
                    auto second = moving_to;
                    first.y = 0.f;
                    second.y = 0.f;
                    glm::vec3 dir = glm::normalize(second - first);
                    dir = dir * (move.speed * static_cast<float>(dt));
                    tran.pos += dir;
                    tran.rot = glm::quatLookAt(glm::normalize(first - second), {0.0f, 1.0f, 0.0f});
                    if (ecs.has<component::Animation>(e)) {
                        auto &anim = ecs.get<component::Animation>(e);
                        anim.animator_.LoadAnimation("WALK", false);
                    }
                } else {
                    if (ecs.has<component::Node>(move.going_to_node)) {
                        ecs.get<component::Node>(move.going_to_node).AlterNodeGrid(node_occupancy::leaving);
                        //node_comp.AlterNodeGrid(node_occupancy::leaving);
                    }
                    if (ecs.has<component::Node>(move.last_node)) {
                        ecs.get<component::Node>(move.last_node).AlterNodeGrid(node_occupancy::vacant);
                    }
                    move.last_node = move.going_to_node;
                    move.going_to_node = move.move_list.front();
                    move.move_list.pop();
                }
            }
        } else {
            if (ecs.has<component::Animation>(e) && move.is_moving) {
                auto &anim = ecs.get<component::Animation>(e);
                if (anim.animator_.loaded_animation_ != nullptr) {
                    if (anim.animator_.loaded_animation_->GetName() == "WALK") {
                        anim.animator_.LoadAnimation("IDLE", false);
                    }
                }
            }
            move.is_moving = false;
        }
    }
}
