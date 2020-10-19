#include "Node.hpp"


Pathing::Node::Node() {
    x      = 0;
    y      = 0;
    parent = nullptr;
}

Pathing::Node::Node(unsigned _x, unsigned _y) {
    x      = _x;
    y      = _y;
    parent = nullptr;
}

int Pathing::Node::fCost() const {
    return this->gCost + this->hCost;
}

auto Pathing::Node::toggleWalkable() -> void {
    this->walkable = !(this->walkable);
}


auto Pathing::Node::setWalkable(bool _walkable) -> void {
    walkable = _walkable;
}
