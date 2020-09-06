#include "PhysicsShape.hpp"


PhysicsShape::PhysicsShape(reactphysics3d::ConvexShape* base_shape, PhysicsShape::ShapeType type) {
	shape_ = base_shape;
    type_ = type;
}

PhysicsShape::ShapeType PhysicsShape::GetShapeType() {
    return type_;
}
