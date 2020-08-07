#pragma once

#include "ECS/ECS.hpp"
#include "Game/BaseState.hpp"
#include "Engine/Physics/PhysicsEngine.hpp"

class PhysicsDemo : public BaseState {
 public:
    PhysicsDemo();
    ~PhysicsDemo() override = default;
    void Init() override;
    void UnInit() override;
    void Display(const glm::mat4& projection, const glm::mat4& view) override;
    void GUIStart() override;
    void GUIEnd() override;
    void Update(double t, double dt) override;
    void FixedUpdate(double t, double dt) override;

 private:
    ECS ecs_ = {};
    PhysicsEngine physics_engine_ = {};
};
