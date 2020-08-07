#pragma once

#include <vector>
#include <memory>

#include <entt/entt.hpp>
#include <glm/matrix.hpp>

class Entity;

class ECS {
public:
    ECS() = default;
    ~ECS() = default;
    Entity& CreateEntity();
    void FixedUpdate(double t, double dt);
    void Update(double t, double dt);
    void Draw(const glm::mat4& projection, const glm::mat4& view,
              const glm::dvec2& camera_pos);

    entt::registry& GetRegistry() {return registry_;};

protected:
    entt::registry registry_ = {};
    std::vector<std::shared_ptr<Entity>> entity_register_ = {};
    friend Entity;
};