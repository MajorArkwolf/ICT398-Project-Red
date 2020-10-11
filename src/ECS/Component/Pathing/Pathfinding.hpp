#pragma once
#include <vector>

#include "Grid.hpp"

/**
 * @namespace Pathing
 * @brief A namespace for the A* pathfinding algorithm
 */
namespace Pathing {

    /**
     * @class Pathfinding
     * @brief A class for finding a path using the A* pathfinding algorithm
     */
    class Pathfinding {
      public:
        static int findDistance(Node *nodeA, Node *nodeB, bool oct);
        static std::vector<Node *> findPath(Grid &nodeGrid, Node *startNode,
                                            Node *endNode, bool oct);
        static bool containsNode(std::vector<Node *> &set, Node *node);
      private:
        static std::vector<Node *> traceRoute(Node *endNode);
        Pathfinding() = default;
    };
};
