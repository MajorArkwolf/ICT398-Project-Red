#pragma once

#include "ECS/Entity.hpp"
#include "Engine/EulerCamera.hpp"

namespace demo {
    class Player {
    public:
        Player();
        ~Player() = default;
        void SetBigPlayer(Entity player);
        void SetLittlePlayer(Entity player);
        engine::Camera &GetActiveCamera();
        Entity &GetActivePlayer();
        void TogglePlayer();
        void Update(double t, double dt);
        void ProcessKeyboardInput(bool forward, bool backward, bool left, bool right, double delta_time);
    private:
        float little_player_offset_ = 2.8f;
        float big_player_offset_ = 150.0f;
        float model_rotation_offset_ = 90.f;
        Entity big_player_;
        Entity little_player_;
        bool big_player_active_ = true;

        float GetActiveOffset();


    };
}