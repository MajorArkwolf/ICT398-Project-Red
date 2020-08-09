#include "Basic.hpp"

namespace component {
    void PhysicBody::AddForce(glm::dvec3 additional_force) {
        velocity += additional_force;
    }

    void PhysicBody::SetVelocity(glm::dvec3 new_velocity) {
        velocity = new_velocity;
    }

    glm::dvec3 PhysicBody::GetVelocity() const {
        return velocity;
    }
}
