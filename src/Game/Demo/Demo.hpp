#pragma once
#include "Game/BaseState.hpp"

class Demo : public BaseState {
public:
    Demo();
    ~Demo() override = default;
    void Init() override;
    void UnInit() override;
    void Display() override;
    void GUIStart() override;
    void GUIEnd() override;
    void Update(double t, double dt) override;
    void FixedUpdate(double t, double dt) override;

private:
    Engine::Camera camera;
};

