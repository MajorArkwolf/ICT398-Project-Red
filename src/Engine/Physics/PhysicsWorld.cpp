#include "PhysicsWorld.hpp"
#include "Engine/Engine.hpp"

using namespace physics;

physics::PhysicsWorld::PhysicsWorld() {
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    world_ = physics_engine.CreatePhysicsWorld();
}

void PhysicsWorld::SetECS(ECS *ecs) {
    this->ecs_ = ecs;
}

PhysicsWorld::PhysicsWorld(ECS *ecs) {
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    world_ = physics_engine.CreatePhysicsWorld();
    ecs_ = ecs;
}

PhysicsWorld::~PhysicsWorld() {
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    physics_engine.DestroyPhysicsWorld(world_);
}


