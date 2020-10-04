#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "ECS/Entity.hpp"
#include "ECS/Component/Pathing/Grid.hpp"

class Shader;

namespace component {
    class Board {
    public:
        Board() = delete;
        Board(ECS *ecs, const glm::vec3 &pos, size_t node_x, size_t node_y, float node_size);
        void Update(double t, double dt);
        void ToggleRenderer();
    private:
        bool render_nodes_ = false;
        glm::vec3 position_{};
        std::vector<std::vector<Entity>> nodes_{};
        float node_size_ = 1.0f;
        size_t node_model_ = 0;
        void BuildBoard(ECS *ecs);

        Pathing::Grid grid_;

    };
}
