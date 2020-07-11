#pragma once
#include <entt/entt.hpp>

class ECS {
public:
    ECS() = default;
    ~ECS() = default;
    void FixedUpdate(double t, double dt);
    void Update(double t, double dt);
    void Draw();
    entt::registry registry;
private:
    bool test = false;
};
