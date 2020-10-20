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
    class Node {
    public:
        void AlterNodeGrid(node_occupancy current);
        node_occupancy GetNodeStatus() {return n_o;}
    private:
        node_occupancy n_o = node_occupancy::vacant;
        Pathing::Node *grid_node = nullptr;
        friend class Board;
    };
}
