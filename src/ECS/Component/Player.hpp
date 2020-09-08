#pragma once
#include "Engine/EulerCamera.hpp"

namespace component{
    struct Player {
        engine::Camera camera{};
        float height = 170.f;

        void Update(double t, double dt) {
            if (camera.position_.y != height) {
                camera.position_.y = height;
            }
        }
    };
}

