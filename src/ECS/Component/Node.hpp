#pragma once

namespace Pathing{
    class Node;
}


enum class node_occupancy {
    vacant,
    occupied,
    leaving,
};

namespace component {
    struct Node {
        node_occupancy n_o = node_occupancy::vacant;
        Pathing::Node *grid_node = nullptr;
    };
}
