#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <queue>
#include "ECS/Entity.hpp"
#include "ECS/Component/Pathing/Grid.hpp"
#include "Engine/Physics/PhysicsWorld.hpp"

class Shader;

namespace component {
    class Board {
    public:
        Board() = delete;
        Board(ECS *ecs, physics::PhysicsWorld *pw ,const glm::vec3 &pos, size_t node_x, size_t node_y, float node_size);
        void Update(double t, double dt);
        void ToggleRenderer();
        std::queue<entt::entity> FindPath(entt::registry &ecs, entt::entity first_node, entt::entity second_node);
        entt::entity GetFirstNode() {return first_board_piece_;}
        entt::entity GetLastNode() {return last_board_piece_;}
        size_t GetNumOfNodes() {return num_of_nodes_;}
        entt::entity GetClosestNode(const glm::vec3& world_cord);
        entt::entity FindClosestNodePoint(entt::registry &reg, glm::vec3 &currentPos, entt::entity going_to);
    private:
        bool render_nodes_ = false;
        glm::vec3 position_{};
        std::vector<std::vector<Entity>> nodes_{};
        float node_size_ = 1.0f;
        size_t node_model_ = 0;
        void BuildBoard(ECS *ecs, physics::PhysicsWorld *pw);
        Pathing::Grid grid_;
        std::unordered_map<Pathing::Node *, entt::entity> node_to_entity_{};
        std::unordered_map<entt::entity, Pathing::Node *> entity_to_node_{};
        size_t num_of_nodes_;
        entt::entity first_board_piece_;
        entt::entity last_board_piece_;
    };
}
