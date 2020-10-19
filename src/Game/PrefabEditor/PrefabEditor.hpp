#pragma once

#include "ECS/ECS.hpp"
#include "Game/BaseState.hpp"
#include "Engine/Physics/PhysicsEngine.hpp"
#include "Game/PrefabEditor/Components/PrefabGUI.hpp"

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
    PrefabGUI prefab_gui_{};
    bool camera_enabled_ = false;
    double distanceFromEntity = 10;
    double currentRotation    = 0;
    void ArcBallCamera(double dx, double dy);
    void UpdateZoom(double movement);
};
