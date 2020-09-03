#include "PhysicsShape.hpp"


PhysicsShape::PhysicsShape(reactphysics3d::ConvexShape* base_shape) {
	shape_ = base_shape;
}
