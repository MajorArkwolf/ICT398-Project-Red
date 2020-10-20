#include "Node.hpp"
#include "ECS/Component/Pathing/Node.hpp"

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
