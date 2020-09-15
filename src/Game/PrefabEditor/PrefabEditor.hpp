#pragma once

#include "ECS/ECS.hpp"
#include "Game/BaseState.hpp"
#include "Engine/Physics/PhysicsEngine.hpp"

class PrefabEditor : public BaseState {
 public:
    PrefabEditor();
    ~PrefabEditor() override = default;
    void Init() override;
    void UnInit() override;
    void Display(Shader *shader, const glm::mat4& projection, const glm::mat4& view) override;
    void GUIStart() override;
    void GUIEnd() override;
    void Update(double t, double dt) override;
    void FixedUpdate(double t, double dt) override;
    void HandleInputData(input::InputEvent inputData, double deltaTime) override;

 private:
    bool forward_ = false, backward_ = false, left_ = false, right_ = false;
    ECS ecs_ = {};
};
