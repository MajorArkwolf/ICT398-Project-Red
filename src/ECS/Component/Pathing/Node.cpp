#include "Node.hpp"

/**
 * @brief Default constructor for Node
 */
Pathing::Node::Node() {
    x      = 0;
    y      = 0;
    parent = nullptr;
}

/**
 * @brief Overloaded constructor for Node
 * @param _x X coordinate to set member x value to
 * @param _y Y coordinate to set member y value to
 */
Pathing::Node::Node(unsigned _x, unsigned _y) {
    x      = _x;
    y      = _y;
    parent = nullptr;
}

/**
 * @brief Returns the fCost of a node
 * @return fcost
 */
int Pathing::Node::fCost() const {
    return this->gCost + this->hCost;
}

/**
 * @brief Toggles whether the current node is walkable
 */
auto Pathing::Node::toggleWalkable() -> void {
    this->walkable = !(this->walkable);
}

/**
 * @brief Sets the nodes walkable value to the passed in parameter
 * @param _walkable Value to set walkable to
 */
auto Pathing::Node::setWalkable(bool _walkable) -> void {
    walkable = _walkable;
}
