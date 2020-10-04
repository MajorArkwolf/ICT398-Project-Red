#pragma once
#include "Engine/EulerCamera.hpp"

namespace component {
    struct Player {
        engine::Camera camera_{};
        float height_ = 170.f;

        void Update(double t, double dt) {
            if (camera_.position_.y != height_) {
                camera_.position_.y = height_;
            }
            if (camera_.position_.x > 152.0f) {
                camera_.position_.x = 152.0f;
            }
            if (camera_.position_.x < -665.0f) {
                camera_.position_.x = -665.0f;
            }
            if (camera_.position_.z < -361.0f) {
                camera_.position_.z = -361.0f;
            }
            if (camera_.position_.z > 760) {
                camera_.position_.z = 760;
            }
        }
    };
}

