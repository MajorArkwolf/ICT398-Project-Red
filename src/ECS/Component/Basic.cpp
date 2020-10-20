#include "Basic.hpp"

namespace component {

void PhysicBody::AddForce(glm::vec3 additional_force) {
    
    added_force += additional_force;
    is_sleeping = false;
}

}// namespace component
