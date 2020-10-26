#include "Player.hpp"

#include <ECS/Component/Model.hpp>

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Player.hpp"
using namespace demo;

Player::Player() {
}

void Player::Update(double t, double dt) {
    auto &player = GetActivePlayer();
    auto &player_comp = player.GetComponent<component::Player>();
    GetActivePlayer().GetComponent<component::Player>().Update(t, dt);
    auto &camera_position = player.GetComponent<component::Player>().camera_.position_;
    auto &position = player.GetComponent<component::Transform>().pos;
    auto &rotation = player.GetComponent<component::Transform>().rot;
    position.y = player.GetComponent<component::Player>().height_;
    camera_position.y = player.GetComponent<component::Player>().camera_height_offset_ + position.y;
    camera_position.x = position.x;
    camera_position.z = position.z;
    rotation = glm::quat(glm::vec3{0.0f, glm::radians(model_rotation_offset_ - player.GetComponent<component::Player>().camera_.yaw_ ), 0.0f});
   if (registry_ != nullptr && holding_object_ == true) {
        auto &transform = registry_->get<component::Transform>(grabbed_object_);
        auto &phys = registry_->get<component::PhysicBody>(grabbed_object_);

        transform.pos = player_comp.camera_.position_ + player_comp.camera_.front_ * 10.f;
    }
}

void Player::SetBigPlayer(Entity player) {
    big_player_ = player;
}

void Player::SetLittlePlayer(Entity player) {
    little_player_ = player;
}

void Player::TogglePlayer() {
    big_player_active_ = !big_player_active_;
    big_player_.GetComponent<component::Model>().draw_model = !big_player_active_;
    little_player_.GetComponent<component::Model>().draw_model = big_player_active_;
}

engine::Camera &Player::GetActiveCamera() {
    return big_player_active_ ? big_player_.GetComponent<component::Player>().camera_ : little_player_.GetComponent<component::Player>().camera_;
}

Entity &Player::GetActivePlayer() {
    return big_player_active_ ? big_player_ : little_player_;
}

float Player::GetActiveOffset() {
    return big_player_active_ ? big_player_offset_ : little_player_offset_;
}

void Player::ProcessKeyboardInput(bool forward, bool backward, bool left,
                                  bool right, double delta_time) {
    auto &player = GetActivePlayer();
    auto &move_speed = player.GetComponent<component::Player>().camera_.movement_speed_;
    auto &position = player.GetComponent<component::Transform>().pos;
    auto &rotation = player.GetComponent<component::Transform>().rot;
    auto front_cam = player.GetComponent<component::Player>().camera_.GetFrontVector();
    auto right_cam = player.GetComponent<component::Player>().camera_.GetRightVector();
    auto &camera_position = player.GetComponent<component::Player>().camera_.position_;
    auto &camera_rotation = player.GetComponent<component::Player>().camera_;

    const auto velocity =
            static_cast<float>(move_speed * delta_time * 1000.0);
    front_cam.y = 0.0f;
    if (forward)
        position += front_cam * velocity;
    if (backward)
        position -= front_cam * velocity;
    if (left)
        position -= right_cam * velocity;
    if (right)
        position += right_cam * velocity;

    camera_position.y = GetActivePlayer().GetComponent<component::Player>().height_;
}

void demo::Player::GrabObject(entt::entity grabbed_object) {

    if (!holding_object_ && registry_ != nullptr && registry_->has<component::PhysicBody>(grabbed_object)) {
        auto &phys = registry_->get<component::PhysicBody>(grabbed_object);
        if (!phys.static_object) {
            phys.is_sleeping = true;
            grabbed_object_ = grabbed_object;
            holding_object_ = true;
        }
    }
}

void demo::Player::ThrowObject(glm::vec3 force) {
    if (holding_object_ && registry_ != nullptr) {
        auto &phys = registry_->get<component::PhysicBody>(grabbed_object_);
        phys.is_sleeping = false;
        phys.AddForce(force);
        DropObject();
    }
}

void demo::Player::DropObject() {
    if (holding_object_ && registry_ != nullptr) {
        auto &phys = registry_->get<component::PhysicBody>(grabbed_object_);
        phys.is_sleeping = false;
        holding_object_ = false;
    }
}

void demo::Player::SetRegistry(entt::registry &the_registry) {
    registry_ = &the_registry;
}
