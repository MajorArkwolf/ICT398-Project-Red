#pragma once 
#include <reactphysics3d/reactphysics3d.h>

/**
* @class Physics Shape
* @brief Class used to represent a particular react shape
*/
namespace physics {
    enum class ShapeType {
        Unknown,
        Box,
        Sphere,
        Capsule
    };

    class PhysicsShape {

        friend class CollisionDetection;

    public:
        ~PhysicsShape() = default;

        ShapeType GetShapeType();

    private:
        PhysicsShape(rp3d::ConvexShape *base_shape, ShapeType type);

        rp3d::ConvexShape *shape_;
        ShapeType type_;
    };
}
