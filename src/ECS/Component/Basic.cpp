#include "Basic.hpp"


namespace component {

    void PhysicBody::AddForce(glm::vec3 additional_force) {

        added_force += additional_force;
        is_sleeping = false;
    }

    void Moving::SetLastNode(entt::registry &reg, entt::entity id) {
        if (reg.has<component::Node>(id)) {
            if (reg.has<component::Node>(last_node)) {
                auto &old_node = reg.get<component::Node>(this->last_node);
                old_node.AlterNodeGrid(node_occupancy::vacant);
            }
            if (reg.has<component::Node>(id)) {
                auto &new_node = reg.get<component::Node>(id);
                new_node.AlterNodeGrid(node_occupancy::leaving);
            }
            this->last_node = id;
        }
    }

    void Moving::SetGoingToNode(entt::registry &reg, entt::entity id) {
        if (reg.has<component::Node>(id)) {
            if (reg.has<component::Node>(going_to_node)) {
                auto &old_node = reg.get<component::Node>(going_to_node);
                old_node.AlterNodeGrid(node_occupancy::leaving);
            }
            if (reg.has<component::Node>(id)) {
                auto &new_node = reg.get<component::Node>(id);
                new_node.AlterNodeGrid(node_occupancy::occupied);
            }
            this->going_to_node = id;
        }
    }
void PhysicBody::AddTorque(glm::vec3 additional_force) {
    added_torque = additional_force;
}

}// namespace component
