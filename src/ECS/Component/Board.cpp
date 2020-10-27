#include "Board.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Engine.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Node.hpp"
#include "ECS/Component/Pathing/Pathfinding.hpp"

component::Board::Board(ECS *ecs, physics::PhysicsWorld *pw, const glm::vec3 &pos, const size_t node_x,
                        const size_t node_y,
                        const float node_size) : grid_(static_cast<unsigned>(node_x), static_cast<unsigned>(node_y)) {
    auto &mm = redengine::Engine::get().model_manager_;
    auto base_path = redengine::Engine::get().GetBasePath();
    base_path = base_path / "res" / "model" / "cube.obj";
    node_model_ = mm.GetModelID(base_path);
    position_ = pos;
    node_size_ = node_size;
    assert(node_x > 0 || node_y > 0);
    nodes_.resize(node_x);
    assert(nodes_.size() == node_x);
    for (auto &node_array : nodes_) {
        node_array.resize(node_y);
        assert(node_array.size() == node_y);
    }
    num_of_nodes_ = (node_x * node_y) - 1;
    BuildBoard(ecs, pw);
}

void component::Board::BuildBoard(ECS *ecs, physics::PhysicsWorld *pw) {
    auto &pe = redengine::Engine::get().GetPhysicsEngine();
    assert(ecs != nullptr);
    size_t count_down = num_of_nodes_;
    float new_pos_x = position_.x;
    auto adjusted_size = node_size_ - 0.1;
    auto box_size = glm::vec3(adjusted_size / 2, adjusted_size / 2, adjusted_size / 2);
    auto box = pe.CreateBoxShape(box_size);
    unsigned x = 0, y = 0;
    for (auto &node_array : nodes_) {
        y = 0;
        float new_pos_z = position_.z;
        for (auto &node : node_array) {
            node = ecs->CreateEntity();
            auto &trans = node.AddComponent<component::Transform>();
            auto &name_comp = node.AddComponent<component::Name>();
            name_comp.name = "Board Node";
            trans.pos.x = new_pos_x;
            trans.pos.y = position_.y;
            trans.pos.z = new_pos_z;
            trans.scale = glm::vec3{node_size_, node_size_, node_size_};
            new_pos_z += node_size_;
            auto &model = node.AddComponent<component::Model>(node_model_);
            model.wire_frame = true;
            model.has_color = true;
            model.draw_model = render_nodes_;
            auto &node_comp = node.AddComponent<component::Node>();
            node_comp.grid_node = grid_.getNode(x, y);
            //TODO: Implement a collision objects here.
            if (pw != nullptr) {
                auto &pb = node.AddComponent<component::PhysicBody>();
                pb.static_object = true;
                pw->AddCollisionBody(node.GetID(), trans.pos, trans.rot);
                pw->AddCollider(node.GetID(), box, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
                pe.SetTrigger(pw, node.GetID(), true);
            }
            node_to_entity_.insert({node_comp.grid_node, node.GetID()});
            entity_to_node_.insert({node.GetID(), node_comp.grid_node});
            ++y;
            if (count_down == num_of_nodes_) {
                first_board_piece_ = node.GetID();
            }
            if (count_down == 0) {
                last_board_piece_ = node.GetID();
            }
            --count_down;
        }
        ++x;
        new_pos_x += node_size_;
    }
}

void component::Board::Update(double t, double dt) {

}

void component::Board::ToggleRenderer() {
    render_nodes_ = !render_nodes_;
    for (auto &node_array : nodes_) {
        for (auto &node : node_array) {
            node.GetComponent<component::Model>().draw_model = render_nodes_;
        }
    }
}

std::queue<entt::entity>
component::Board::FindPath(entt::registry &ecs, entt::entity first_node, entt::entity second_node) {
    std::queue<entt::entity> node_list = {};
    if (ecs.has<component::Node>(first_node) && ecs.has<component::Node>(second_node)) {
        auto *node_comp_first = ecs.get<component::Node>(first_node).grid_node;
        auto *node_comp_second = ecs.get<component::Node>(second_node).grid_node;
        auto result = Pathing::Pathfinding::findPath(grid_, node_comp_first, node_comp_second, true);
        for (auto &e : result) {
            node_list.push(node_to_entity_.at(e));
        }
    }
    return node_list;
}
entt::entity component::Board::GetClosestNode(const glm::vec3& world_cord) {
    entt::entity result;
    if (nodes_.empty() || nodes_.at(0).empty()) {
        assert(false);
    }
    Entity closest_entity = nodes_.at(0).at(0);
    auto& first_tran = closest_entity.GetComponent<component::Transform>();
    float distance = glm::distance(first_tran.pos, world_cord);
    for (auto &node_array : nodes_) {
        for (auto& node : node_array) {
            auto& tran = node.GetComponent<component::Transform>();
            float new_dist = glm::distance(tran.pos, world_cord);
            if (new_dist < distance) {
                distance = new_dist;
                result = node.GetID();
            }
        }
    }
    return result;
}
