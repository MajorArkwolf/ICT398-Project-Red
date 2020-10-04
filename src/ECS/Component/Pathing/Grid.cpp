#include "Grid.hpp"

using Pathing::Grid;
using Pathing::Node;

/**
 * @brief Default constructor for grid
 * Creates a grid
 */
Grid::Grid() {
    this->resizeGrid(this->gridSizeX, this->gridSizeY);
}

/**
 * @brief Overloaded constructor for grid, creates a grid using the passed in parameters
 * @param _gridSizeX X size of grid to create
 * @param _gridSizeY Y size of grid to create
 */
Grid::Grid(unsigned _gridSizeX, unsigned _gridSizeY) {
    this->resizeGrid(_gridSizeX, _gridSizeY);
}

/**
 * @brief Resets the costs of all nodes in grid, needs to happen for pathfinding
 */
void Pathing::Grid::resetGridCosts() {
    for (auto &x : nodeGrid) {
        for (auto &y : x) {
            y.gCost  = 0;
            y.hCost  = 0;
            y.parent = nullptr;
        }
    }
}

/**
 * @brief Resets grid back to base
 */
void Pathing::Grid::resetGrid() {
    for (auto &x : nodeGrid) {
        for (auto &y : x) {
            y.gCost    = 0;
            y.hCost    = 0;
            y.walkable = false;
            y.parent   = nullptr;
            y.occupied = false;
        }
    }
}

/**
 * @brief Returns a pointer to the node at the given position
 * @param pos glm unsigned vec 2 of position of node to return
 * @return Pointer to node at given coordinate
 */
Node *Pathing::Grid::getNode(glm::uvec2 pos) {
    if (pos.x > gridSizeX || pos.y > gridSizeY) {
        return nullptr;
    }
    return &nodeGrid[pos.x][pos.y];
}

/**
 * @brief Resiszes the grid to the passed in values
 * @param _gridSizeX X size of grid to create
 * @param _gridSizeY Y size of grid to create
 */
void Grid::resizeGrid(unsigned _gridSizeX, unsigned _gridSizeY) {
    nodeGrid.resize(_gridSizeX);

    for (auto &array : nodeGrid) {
        array.resize(_gridSizeY);
    }

    gridSizeX = _gridSizeX;
    gridSizeY = _gridSizeY;

    for (unsigned x = 0; x < gridSizeX; x++) {
        for (unsigned y = 0; y < gridSizeY; y++) {
            nodeGrid[x][y].x = x;
            nodeGrid[x][y].y = y;
        }
    }
}

/**
 * @brief Returns all 8 neighbours of a given node if they exist
 * @param node The node to get neighbours from
 * @return A vector containing pointers to all the node neighbours
 */
vector<Node *> Grid::getNeighbours(Node &node) {
    return this->getNeighbours(node, 1, 1);
}

/**
 * @brief Returns the specified number of neighbours of a given node if they exist
 * @param node The node to get neighbours from
 * @param radius The radius to get neighbours from, Use 1 for default
 * @param oct True = return all 8 neighbours, False = return the 4 neighbours in each cardinal direction
 * @return A vector containing pointers to all the node neighbours
 */
vector<Node *> Grid::getNeighbours(Node &node, int radius, bool oct) {
    vector<Node *> newList;
    long long int nodeX = node.x;
    long long int nodeY = node.y;

    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            if (x != 0 || y != 0) {
                if (!(nodeX + x <= -1 || nodeX + x >= gridSizeX ||
                      nodeY + y <= -1 || nodeY + y >= gridSizeY)) {
                    if (!oct) {
                        if ((y == 0 || x == 0)) {
                            newList.push_back(&nodeGrid[static_cast<unsigned>(
                                nodeX + x)][static_cast<unsigned>(nodeY + y)]);
                        }
                    } else {
                        newList.push_back(&nodeGrid[static_cast<unsigned>(
                            nodeX + x)][static_cast<unsigned>(nodeY + y)]);
                    }
                }
            }
        }
    }

    return newList;
}
