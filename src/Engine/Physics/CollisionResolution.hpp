#pragma once
#include <queue>
#include "PhysicsData.hpp"

class ECS;
namespace physics {

class CollisionResolution {
   public:
    void Resolve(std::queue<PhysicsCollisionData>& queue, double t, double dt);
    CollisionResolution() = default;
    ~CollisionResolution() = default;
    void SetECS(ECS* ecs);
    private:
    ECS* ecs_ = nullptr;
};
}// namespace physics
