#pragma once
#include <glm/vec3.hpp>

enum class CollisionShapeIdentity {
    Square,
    Sphere,
    Capsule
};

struct CollisionShape {
    CollisionShapeIdentity collision_shape;
    glm::vec3 coordinate_to_local = {};
    glm::vec3 size_of_shape = {};
    float radius = 0.0f;
};