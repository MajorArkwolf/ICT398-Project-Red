#pragma once
#include <entt/entt.hpp>
#include <glm/matrix.hpp>

class ECS {
public:
    ECS() = default;
    ~ECS() = default;
    void FixedUpdate(double t, double dt);
    void Update(double t, double dt);
    void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::dvec2& cameraPos);
    entt::registry registry;
};
