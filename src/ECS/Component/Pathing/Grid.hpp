#pragma once
#include <vector>

#include "Node.hpp"
#include "glm/vec2.hpp"

using std::vector;

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
        Grid();
        Grid(unsigned _gridSizeX, unsigned _gridSizeY);

        void resetGridCosts();
        void resetGrid();
        Node *getNode(glm::uvec2 pos);
        Node *getNode(unsigned x, unsigned y);
        void resizeGrid(unsigned x, unsigned y);

        /// 2Dimensional Vector of nodes
        vector<vector<Node>> nodeGrid;

        /// The number of columns of nodes
        unsigned gridSizeX = 20;

        /// The number of rows of nodes
        unsigned gridSizeY = 20;

        vector<Node *> getNeighbours(Node &_node);
        vector<Node *> getNeighbours(Node &_node, int radius, bool oct);
    };
};
