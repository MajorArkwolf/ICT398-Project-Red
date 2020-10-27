#pragma once
#include <entt/entt.hpp>
#include "Engine/Physics/PhysicsData.hpp"

namespace Pathing{
    class Node;
}


enum class node_occupancy {
    vacant,
    occupied,
    leaving,
};

namespace component {
    class Node {
    public:
        void AlterNodeGrid(node_occupancy current);
        node_occupancy GetNodeStatus() {return n_o;}
        void onTrigger(entt::registry& reg, entt::entity ent, PhysicsTriggerData::Event event);
        entt::entity current_obj;
    private:
        node_occupancy n_o = node_occupancy::vacant;
        Pathing::Node *grid_node = nullptr;
        friend class Board;
    };
}
