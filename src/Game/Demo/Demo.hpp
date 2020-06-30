#pragma once
#include "Game/BaseState.hpp"
class Demo : public BaseState {
public:
    Demo();
    ~Demo() override = default;
    void Init() override;
    void unInit() override;
    void display() override;
    void GUIStart() override;
    void GUIEnd() override;

private:
    Engine::Camera camera;
};

