#include "Grid.hpp"

using Pathing::Grid;
using Pathing::Node;


Grid::Grid() {
    this->resizeGrid(this->gridSizeX, this->gridSizeY);
}

Grid::Grid(unsigned _gridSizeX, unsigned _gridSizeY) {
    this->resizeGrid(_gridSizeX, _gridSizeY);
}

void Pathing::Grid::resetGridCosts() {
    for (auto &x : nodeGrid) {
        for (auto &y : x) {
            y.gCost  = 0;
            y.hCost  = 0;
            y.parent = nullptr;
        }
    }
}

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

Node *Pathing::Grid::getNode(glm::uvec2 pos) {
    return getNode(pos.x, pos.y);
}

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

vector<Node *> Grid::getNeighbours(Node &node) {
    return this->getNeighbours(node, 1, 1);
}


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

Node *Pathing::Grid::getNode(unsigned int x, unsigned int y) {
    if (x >= gridSizeX || y >= gridSizeY) {
        assert(false);
        return nullptr;
    }
    return &nodeGrid[x][y];
}
