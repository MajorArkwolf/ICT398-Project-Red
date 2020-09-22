#pragma once

enum class node_occupancy {
    vacant,
    occupied,
    leaving,
};

namespace component {
    struct node {
        node_occupancy n_o = node_occupancy::vacant;
    };
}
