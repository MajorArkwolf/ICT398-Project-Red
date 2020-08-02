#pragma once
#include "Game/BaseState.hpp"
#include "ECS/Ecs.hpp"

class Demo : public BaseState {
public:
    Demo();
    ~Demo() override = default;
    void Init() override;
    void UnInit() override;
    void Display(glm::mat4 projection, glm::mat4 view) override;
    void GUIStart() override;
    void GUIEnd() override;
    void Update(double t, double dt) override;
    void FixedUpdate(double t, double dt) override;
    void HandleInputData(input::InputEvent inputData, double deltaTime) override;

private:
    ECS ecs = {};
};

