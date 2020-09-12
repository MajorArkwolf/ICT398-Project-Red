#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <entt/entt.hpp>
#include <glm/matrix.hpp>

class Entity;

class Shader;

/**
 * A lightweight abstraction used to help keep some API calls simple
 */
class ECS {
public:

    /**
     * Default Constructor
     */
    ECS() = default;

    /**
     * Default Destructor
     */
    ~ECS() = default;

    /**
     * Create Entity
     * @return an abstract entity of ENTT.
     */
    Entity& CreateEntity();

    /**
     * Fixed Update
     * @param t time since engine started.
     * @param dt time since last frame aka delta time.
     */
    void FixedUpdate(double t, double dt);

    /**
     * Update
     * @param t time since engine started.
     * @param dt time since last frame aka delta time.
     */
    void Update(double t, double dt);

    /**
     * Draw call for all entities that have a model.
     * @param shader shader to be used on that given pass.
     * @param projection the projection matrix of the camera.
     * @param view the view matrix of the camera.
     */
    void Draw(Shader *shader, const glm::mat4& projection, const glm::mat4& view);

    entt::registry& GetRegistry() {return registry_;};

protected:
    entt::registry registry_ = {};
    std::vector<std::shared_ptr<Entity>> entity_register_ = {};
    std::unordered_map<entt::entity, std::shared_ptr<Entity>> entity_lookup_ = {};
    friend Entity;
};