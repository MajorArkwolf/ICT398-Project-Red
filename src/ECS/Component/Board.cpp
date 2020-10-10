#include "Board.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Engine.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Node.hpp"

component::Board::Board(ECS *ecs, const glm::vec3 &pos, const size_t node_x, const size_t node_y,
                        const float node_size) : grid_(node_x, node_y) {
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
            trans.scale = 0.0;
            new_pos_z += node_size_;
            auto &model = node.AddComponent<component::Model>(node_model_);
            model.wire_frame = true;
            model.has_color = true;
            model.draw_model = render_nodes_;
            node.AddComponent<component::node>();
        }
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


