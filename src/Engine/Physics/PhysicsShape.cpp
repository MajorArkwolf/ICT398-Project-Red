#include "PhysicsShape.hpp"


physics::PhysicsShape::PhysicsShape(reactphysics3d::ConvexShape* base_shape, ShapeType type) {
	shape_ = base_shape;
    type_ = type;
}

physics::ShapeType physics::PhysicsShape::GetShapeType() {
    return type_;
}
