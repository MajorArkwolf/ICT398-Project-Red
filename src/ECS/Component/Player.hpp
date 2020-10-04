#pragma once
#include "Engine/EulerCamera.hpp"
#include <entt/entt.hpp>

namespace component {
    struct Player {
        engine::Camera camera{};
        float height = 170.f;
        entt::entity selected_entity;

        void Update(double t, double dt) {
            if (camera.position_.y != height) {
                camera.position_.y = height;
            }
            if (camera.position_.x > 152.0f) {
                camera.position_.x = 152.0f;
            }
            if (camera.position_.x < -665.0f) {
                camera.position_.x = -665.0f;
            }
            if (camera.position_.z < -361.0f) {
                camera.position_.z = -361.0f;
            }
            if (camera.position_.z > 760) {
                camera.position_.z = 760;
            }
        }
    };
}

