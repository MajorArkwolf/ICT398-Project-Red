#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <entt/entt.hpp>

class CollisionDetection {
public:
    CollisionDetection();
    void AddCollisionBody(entt::entity entity_id);
private:
    reactphysics3d::PhysicsCommon physicsCommon_{};
    reactphysics3d::PhysicsWorld* world_ = nullptr;
    std::unordered_map<entt::entity, reactphysics3d::CollisionBody> entity_to_collision_;
    std::unordered_map<reactphysics3d::CollisionBody, entt::entity> collision_to_entity_;

    void AddBodyAndEntt(entt::entity& entity, reactphysics3d::CollisionBody& coll_body);
};
