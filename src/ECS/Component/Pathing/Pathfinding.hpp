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
        /**
         * @brief A namespace for the A* pathfinding algorithm
         * @param nodeA The starting node to find the path between
         * @param nodeB The ending node to find the path between
         * @param oct Whether to find distance assuming traveling to all 8 surrounding nodes rather than 4
         * @return The distance between two nodes on the grid
         */
        static int findDistance(Node *nodeA, Node *nodeB, bool oct);
        /**
         * @brief A method to find a path between two nodes using A* pathfinding
         * @param nodeGrid The grid to read from
         * @param startNode The node to start from
         * @param endNode The node to end at
         * @param oct Whether the path will assume diagonal movement or not
         * @return The path from the start node to the end node in vector form
         */
        static std::vector<Node *> findPath(Grid &nodeGrid, Node *startNode,
                                            Node *endNode, bool oct);
        /**
         * @brief A namespace for the A* pathfinding algorithm
         * @param set The vector to search in for node
         * @param node The node to search for in set
         * @return Returns true if node is contained within set
         */
        static bool containsNode(std::vector<Node *> &set, Node *node);
      private:
        /**
         * @brief Returns the route from the end node to the start node in a vector
         * @param endNode The node to read the path backwards from
         * @return A vector containing the current path, ordered from start to end
         */
        static std::vector<Node *> traceRoute(Node *endNode);
        Pathfinding() = default;
    };
};
