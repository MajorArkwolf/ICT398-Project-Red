#include "Board.hpp"
#include <Engine/Renderer/Shader.hpp>
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"

component::Board::Board(ECS *ecs, const glm::vec3 &pos, const size_t node_x, const size_t node_y,
                        const float node_size) {
    position_ = pos;
    node_size_ = node_size;
    assert(node_x > 0 || node_y > 0);
    nodes_.resize(node_x);
    assert(nodes_.size() == node_x);
    for (auto &node_array : nodes_) {
        node_array.resize(node_y);
        assert(node_array.size() == node_y);
    }
    BuildBoard(ecs);
}

void component::Board::BuildBoard(ECS *ecs) {
    assert(ecs != nullptr);
    float new_pos_x = position_.x;
    for (auto &node_array : nodes_) {
        float new_pos_z = position_.z;
        for (auto &node : node_array) {
            node = ecs->CreateEntity();
            auto &trans = node.AddComponent<component::Transform>();
            trans.pos.x = new_pos_x;
            trans.pos.y = position_.y;
            trans.pos.z = new_pos_z;
            trans.scale = glm::vec3{node_size_, node_size_, node_size_};
            new_pos_z += node_size_;
            auto &model = node.AddComponent<component::Model>(node_model_);
        }
        new_pos_x += node_size_;
    }
}

void component::Board::Update(double t, double dt) {

}


