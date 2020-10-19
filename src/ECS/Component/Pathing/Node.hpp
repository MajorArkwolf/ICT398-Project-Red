#pragma once

#include "ECS/ECS.hpp"

/**
 * @namespace Pathing
 * @brief A namespace for the A* pathfinding algorithm
 */
namespace Pathing {

    /**
     * @class Node
     * @brief A class for representing a grid space on the grid
     */
    class Node {
      public:
        /**
         * @brief Default constructor for Node
         */
        Node();
        /**
         * @brief Overloaded constructor for Node
         * @param _x X coordinate to set member x value to
         * @param _y Y coordinate to set member y value to
         */
        Node(unsigned _x, unsigned _y);

        /// Parent node for pathfinding algorithm
        Node *parent = nullptr;
        /// gCost + hCost;
        /**
         * @brief Returns the fCost of a node
         * @return fcost
         */
        int fCost() const;
        /// Distance from end node
        int hCost = 0;
        /// Distance from starting node
        int gCost = 0;
        /// X location in grid
        unsigned x = 0;
        /// Y location in grid
        unsigned y = 0;
        /// If node is traversable then walkable == 1
        bool walkable = true;
		/// Entities use this variable to ensure they are not stepping on each others squares.
		bool occupied = false;

        /**
         * @brief Toggles whether the current node is walkable
         */
        auto toggleWalkable() -> void;
        /**
         * @brief Sets the nodes walkable value to the passed in parameter
         * @param _walkable Value to set walkable to
         */
        auto setWalkable(bool _walkable) -> void;
    };
};
