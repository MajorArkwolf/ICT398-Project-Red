#pragma once
#include <queue>
#include "PhysicsData.hpp"

class ECS;
class Entity;
namespace physics {

class CollisionResolution {
   public:
    void Resolve(std::queue<PhysicsCollisionData>& queue, double t, double dt);
    CollisionResolution() = default;
    ~CollisionResolution() = default;
    void SetECS(ECS* ecs);
    private:
    void ResolvePlayerCollision(PhysicsCollisionData& collision, std::shared_ptr<Entity> player, std::shared_ptr<Entity> other);
    void ResolvePhysicsCollision(PhysicsCollisionData& collision, std::shared_ptr<Entity> first_object, std::shared_ptr<Entity> second_object);
    ECS* ecs_ = nullptr;
};
}// namespace physics
