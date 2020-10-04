
#include "Pathfinding.hpp"

#include <algorithm>
#include <iostream>
#include <math.h>
#include <set>

using Pathing::Node;
using Pathing::Pathfinding;

/**
 * @brief A namespace for the A* pathfinding algorithm
 * @param nodeA The starting node to find the path between
 * @param nodeB The ending node to find the path between
 * @param oct Whether to find distance assuming traveling to all 8 surrounding nodes rather than 4
 * @return The distance between two nodes on the grid
 */
int Pathing::Pathfinding::findDistance(Node &nodeA, Node &nodeB, bool oct) {

    int diagonalCost = 20;
    int straightCost = 10;

    if (oct) {
        diagonalCost = 14;
    }

    int dstX = abs(static_cast<int>(nodeA.x) - static_cast<int>(nodeB.x));
    int dstY = abs(static_cast<int>(nodeA.y) - static_cast<int>(nodeB.y));

    if (dstX > dstY) {
        return diagonalCost * dstY + straightCost * (dstX - dstY);
    }
    return diagonalCost * dstX + straightCost * (dstY - dstX);
}

/**
 * @brief A namespace for the A* pathfinding algorithm
 * @param set The vector to search in for node
 * @param node The node to search for in set
 * @return Returns true if node is contained within set
 */
bool Pathing::Pathfinding::containsNode(std::vector<Node *> &set, Node *node) {
    //return std::any_of(set.cbegin(), set.cend(), node);
    for (const auto &m : set) {
        if (node == m) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Returns the route from the end node to the start node in a vector
 * @param endNode The node to read the path backwards from
 * @return A vector containing the current path, ordered from start to end
 */
std::vector<Node *> Pathing::Pathfinding::traceRoute(Node *endNode) {

    auto path = std::vector<Node *>();
    Node *currentNode = endNode;

    while (currentNode->parent != nullptr) {
        path.push_back(currentNode);
        currentNode = currentNode->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

/**
 * @brief A method to find a path between two nodes using A* pathfinding
 * @param nodeGrid The grid to read from
 * @param startNode The node to start from
 * @param endNode The node to end at
 * @param oct Whether the path will assume diagonal movement or not
 * @return The path from the start node to the end node in vector form
 */
 //TODO: This is wildly ineffecient, we could fix this.
std::vector<Node *> Pathing::Pathfinding::findPath(Grid &nodeGrid, Node &startNode,
                                                   Node &endNode, bool oct) {
    std::vector<Node *> openSet;
    std::vector<Node *> closedSet;
    nodeGrid.resetGridCosts();

	startNode.occupied = false;
    endNode.occupied   = false;

    openSet.push_back(&startNode);

    while (!openSet.empty()) {

        Node *currentNode = openSet[0];

        // Find best node to use from open set
        for (unsigned i = 1; i < openSet.size(); i++) {
            if (openSet[i]->fCost() < currentNode->fCost() ||
                openSet[i]->fCost() == currentNode->fCost()) {
                if (openSet[i]->hCost < currentNode->hCost)
                    currentNode = openSet[i];
            }
        }

        // Remove current node from openSet
        for (auto it = openSet.begin(); it != openSet.end();) {
            if (*it == currentNode) {
                it = openSet.erase(it);
            } else {
                ++it;
            }
        }

        closedSet.push_back(currentNode);

        // Path find
        if (currentNode == &endNode) {
            startNode.occupied = true;
            endNode.occupied   = true;
            return traceRoute(&endNode);

        }

        // For each surrounding node
        for (auto neighbour : nodeGrid.getNeighbours(*currentNode, 1, oct)) {

            // Skip to next neighbour if current neighbour is unwalkable or in the closedSet
            if (containsNode(closedSet, neighbour) || !neighbour->walkable || neighbour->occupied) {
                continue;
            }

            // Calculate costs and set parents
            int newCostToNeighbour =
                currentNode->gCost + findDistance(*currentNode, *neighbour, oct);
            // std::cout << newCostToNeighbour << std::endl;

            if (newCostToNeighbour < neighbour->gCost ||
                !containsNode(openSet, neighbour)) {
                neighbour->gCost  = newCostToNeighbour;
                neighbour->hCost  = findDistance(*neighbour, endNode, oct);
                neighbour->parent = currentNode;
            }

            // If the neighbour is already in the openList, skip to next neighbour
            for (auto n : openSet) {
                if (neighbour == n && neighbour->gCost > n->gCost) {
                    continue;
                }
            }
            // If loop reaches this point, then neighbour gets pushed to openSet
            openSet.push_back(neighbour);
        }
    }
    startNode.occupied = true;
    endNode.occupied   = true;
    std::vector<Node *> emptyList;
    return emptyList;
}
