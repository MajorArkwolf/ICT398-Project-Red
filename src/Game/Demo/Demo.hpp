#pragma once

#include "ECS/ECS.hpp"
#include "Game/BaseState.hpp"
#include "Game/Demo/Components/Player.hpp"

class Demo : public BaseState {
public:
    Demo();
    ~Demo() override = default;
    void Init() override;
    void UnInit() override;
    void Display(Shader *shader, const glm::mat4& projection, const glm::mat4& view) override;
    void GUIStart() override;
    void GUIEnd() override;
    void Update(double t, double dt) override;
    void FixedUpdate(double t, double dt) override;
    void HandleInputData(input::InputEvent inputData, double deltaTime) override;

private:
    ECS ecs_ = {};
    bool forward_ = false, backward_ = false, left_ = false, right_ = false;
    demo::Player player_{};
};
