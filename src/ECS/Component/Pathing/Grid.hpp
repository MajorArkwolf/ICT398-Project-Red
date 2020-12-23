#pragma once
#include <vector>

#include "Node.hpp"
#include "glm/vec2.hpp"

/**
 * @namespace Pathing
 * @brief A namespace for the A* pathfinding algorithm
 */
namespace Pathing {

    /**
     * @class Grid
     * @brief A class used to represent a grid using a vector of a vector of nodes
     */
    class Grid {
      public:
        /**
         * @brief Default constructor for grid
         * Creates a grid
         */
        Grid();
        /**
         * @brief Overloaded constructor for grid, creates a grid using the passed in parameters
         * @param _gridSizeX X size of grid to create
         * @param _gridSizeY Y size of grid to create
         */
        Grid(unsigned _gridSizeX, unsigned _gridSizeY);
        /**
         * @brief Resets the costs of all nodes in grid, needs to happen for pathfinding
         */
        void resetGridCosts();
        /**
         * @brief Resets grid back to base
         */
        void resetGrid();
        /**
         * @brief Returns a pointer to the node at the given position
         * @param pos glm unsigned vec 2 of position of node to return
         * @return Pointer to node at given coordinate
         */
        Node *getNode(glm::uvec2 pos);
        /**
         * Returns a pointer to the node at the given position
         * @param x unsigned x of position of node to return
         * @param y unsigned y of position of node to return
         * @return Pointer to node at given coordinate
         */
        Node *getNode(unsigned x, unsigned y);
        /**
         * @brief Resiszes the grid to the passed in values
         * @param _gridSizeX X size of grid to create
         * @param _gridSizeY Y size of grid to create
         */
        void resizeGrid(unsigned x, unsigned y);

        /// 2Dimensional Vector of nodes
        std::vector<std::vector<Node>> nodeGrid;

        /// The number of columns of nodes
        unsigned gridSizeX = 20;

        /// The number of rows of nodes
        unsigned gridSizeY = 20;
        /**
         * @brief Returns all 8 neighbours of a given node if they exist
         * @param node The node to get neighbours from
         * @return A vector containing pointers to all the node neighbours
         */
        std::vector<Node *> getNeighbours(Node &_node);
        /**
         * @brief Returns the specified number of neighbours of a given node if they exist
         * @param node The node to get neighbours from
         * @param radius The radius to get neighbours from, Use 1 for default
         * @param oct True = return all 8 neighbours, False = return the 4 neighbours in each cardinal direction
         * @return A vector containing pointers to all the node neighbours
         */
        std::vector<Node *> getNeighbours(Node &_node, int radius, bool oct);
    };
};
