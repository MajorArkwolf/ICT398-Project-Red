#pragma once
#include <variant>
#include <reactphysics3d/reactphysics3d.h>

class PhysicsShape {
    friend class CollisionDetection;
   public:
    ~PhysicsShape() = default;

   private:
    PhysicsShape(rp3d::ConvexShape* base_shape);
    rp3d::ConvexShape* shape_;

};