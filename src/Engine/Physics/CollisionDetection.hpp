#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class ECS;

class CollisionDetection {
public:
    /**
     * Default constructor, creates a physics world on construction.
     */
    CollisionDetection();
    ~CollisionDetection();

    void AddCollisionBody(entt::entity& entity_id, const glm::vec3& pos, const glm::quat& rot);
    void UpdateCollisionBody(entt::entity& entity_id, const glm::vec3& pos, const glm::quat& rot);
private:
    reactphysics3d::PhysicsCommon physicsCommon_{};
    reactphysics3d::PhysicsWorld* world_ = nullptr;
    std::unordered_map<entt::entity, reactphysics3d::CollisionBody*> entity_collision_coupling_ = {};
    std::unordered_map<reactphysics3d::CollisionBody*, entt::entity> collision_entity_coupling_ = {};

    /**
     * Creates a relationship bidirectional relationship between entity and a collision body for fast lookup times.
     * @param entity the entity id
     * @param coll_body pointer to the react physics body
     */
    void AddBodyAndEntt(entt::entity& entity, reactphysics3d::CollisionBody* coll_body);
};
