#pragma once
#include <entt/entt.hpp>
#include <glm/matrix.hpp>
#include <vector>

class Entity;

class ECS {
public:
    ECS() = default;
    ~ECS() = default;
    Entity& CreateEntity();
    void FixedUpdate(double t, double dt);
    void Update(double t, double dt);
    void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::dvec2& cameraPos);
    entt::registry& GetRegistry() {return registry;};
protected:
    entt::registry registry = {};
    std::vector<std::shared_ptr<Entity>> entityRegister = {};

    friend Entity;
};
