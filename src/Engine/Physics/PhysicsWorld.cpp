#include "PhysicsWorld.hpp"
#include "Engine/Engine.hpp"

using namespace physics;

physics::PhysicsWorld::PhysicsWorld() {
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    world_ = physics_engine.CreatePhysicsWorld();
    event_listener_ = RedEngineEventListener(&collision_entity_coupling_);
    world_->setEventListener(&event_listener_);
}

void PhysicsWorld::SetECS(ECS *ecs) {
    this->ecs_ = ecs;
}

PhysicsWorld::PhysicsWorld(ECS *ecs) {
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    world_ = physics_engine.CreatePhysicsWorld();
    event_listener_ = RedEngineEventListener(&collision_entity_coupling_);
    world_->setEventListener(&event_listener_);
    ecs_ = ecs;
}

PhysicsWorld::~PhysicsWorld() {
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    physics_engine.DestroyPhysicsWorld(world_);
}


