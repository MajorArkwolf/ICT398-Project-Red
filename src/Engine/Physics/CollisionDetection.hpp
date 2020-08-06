#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class ECS;

class RedEngineEventListener : public reactphysics3d::EventListener {
public:
    RedEngineEventListener() = default;
    virtual void onContact(const reactphysics3d::CollisionCallback::CallbackData& callbackData);
};

class CollisionDetection {
public:
    /**
     * Default constructor, creates a physics world on construction.
     */
    CollisionDetection();
    ~CollisionDetection();

    void AddCollisionBody(const entt::entity& entity_id, const glm::vec3& pos, const glm::quat& rot);
    void UpdateCollisionBody(const entt::entity& entity_id, const glm::vec3& pos, const glm::quat& rot);
    void DeleteCollisionBody(const entt::entity& entity_id);

private:
    reactphysics3d::PhysicsCommon physics_common_{};
    reactphysics3d::PhysicsWorld* world_ = nullptr;
    RedEngineEventListener event_listener = {};
    std::unordered_map<entt::entity, reactphysics3d::CollisionBody*> entity_collision_coupling_ = {};
    std::unordered_map<reactphysics3d::CollisionBody*, entt::entity> collision_entity_coupling_ = {};

    /**
     * Creates a relationship bidirectional relationship between entity and a collision body for fast lookup times.
     * @param entity the entity id
     * @param coll_body pointer to the react physics body
     */
    void AddBodyAndEntt(entt::entity& entity, reactphysics3d::CollisionBody* coll_body);
};
