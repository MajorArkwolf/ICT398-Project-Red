#pragma once
#include <queue>

#include "PhysicsData.hpp"

class ECS;
class Entity;
namespace physics {

class CollisionResolution {
   public:
    /**
    * @brief Resolves the collisions between two colliding objects with physics bodies
    * @param queue The queue containing all of the collisions in the current dyunamics update
    * @param t Engine time
    * @param dt Time since last frame in seconds
    */
    void Resolve(std::queue<PhysicsCollisionData>& queue, double t, double dt);
    CollisionResolution() = default;
    ~CollisionResolution() = default;

    /**
    * @brief Sets the ecs to use
    * @param ecs the ecs to use
    */
    void SetECS(ECS* ecs);

   private:
    /**
    * @brief Resolves the collisions between two colliding objects with physics bodies
    * @param queue The queue containing all of the collisions in the current dyunamics update
    * @param t Engine time
    * @param dt Time since last frame in seconds
    */
    void ResolvePhysicsCollision(PhysicsCollisionData& collision, std::shared_ptr<Entity> first_object, std::shared_ptr<Entity> second_object, double dt);
    ECS* ecs_ = nullptr;
};
}// namespace physics
