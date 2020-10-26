#pragma once
#include "Engine/EulerCamera.hpp"
#include <entt/entt.hpp>

namespace component {
    struct Player {
        engine::Camera camera_{};
        float height_ = 0.0f;
        float camera_height_offset_ = 170.0f;
        entt::entity selected_entity;

        void Update([[maybe_unused]] double t, [[maybe_unused]] double dt) {

        }
    };
}

