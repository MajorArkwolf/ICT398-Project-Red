#pragma once
#include <variant>
#include <reactphysics3d/reactphysics3d.h>

/**
* @class Physics Shape
* @brief Class used to represent a particular react shape
*/
class PhysicsShape {
    friend class CollisionDetection;
   public:
    ~PhysicsShape() = default;

   private:
    PhysicsShape(rp3d::ConvexShape* base_shape);
    rp3d::ConvexShape* shape_;

};