#pragma once
#include "Engine/Physics/CollisionDetection.hpp"

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();
    PhysicsEngine(PhysicsEngine& p) = delete;
    PhysicsEngine(PhysicsEngine&& p) = default;
    PhysicsEngine& operator= (PhysicsEngine p) = delete;
    PhysicsEngine& operator= (PhysicsEngine&& p) = default;
    CollisionDetection collision_detection_ = {};
};

