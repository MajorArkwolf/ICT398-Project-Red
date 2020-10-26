#include "Node.hpp"
#include "ECS/Component/Pathing/Node.hpp"
#include "ECS/Component/Basic.hpp"

void component::Node::AlterNodeGrid(node_occupancy current) {
    n_o = current;
    if (current == node_occupancy::occupied || current == node_occupancy::leaving) {
        if (grid_node != nullptr) {
            grid_node->occupied = true;
        }
    } else {
        if (grid_node != nullptr) {
            grid_node->occupied = false;
        }
    }
}

void component::Node::onTrigger(entt::registry &reg, entt::entity ent, PhysicsTriggerData::Event event) {
    if (reg.has<component::InteractableObject>(ent)) {
        if (reg.get<component::InteractableObject>(ent).type != InteractableObject::Type::npc) {
            if (event == PhysicsTriggerData::Event::start) {
                this->AlterNodeGrid(node_occupancy::occupied);
                current_obj = ent;
            } else if (event == PhysicsTriggerData::Event::exit) {
                this->AlterNodeGrid(node_occupancy::vacant);
            }
        }
    }
}
