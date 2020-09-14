#include "PhysicsEngine.hpp"
#include "Engine/Engine.hpp"
#include "ECS/Component/Player.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/ECS.hpp"

using namespace physics;

void PhysicsEngine::FixedUpdate(double t, double dt) {
    collision_detection_.FixedUpdate(t, dt);
}

void PhysicsEngine::Update(double t, double dt) {
    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    auto &ecs = physics_world.ecs_;
    if (ecs != nullptr) {
        auto &registry = ecs->GetRegistry();
        auto entities = registry.view<component::Transform, component::PhysicBody>();

        for (auto &e : entities) {
            auto &tran = entities.get<component::Transform>(e);
            //collision_detection_.UpdateCollisionBody(e, tran.pos, tran.rot);
            physics_world.UpdateCollisionBody(e, tran.pos, tran.rot);
        }

        auto players = registry.view<component::Player>();
        for (auto &e : players) {
            auto &p = players.get<component::Player>(e);
            physics_world.UpdateCollisionBody(e, p.camera.position_, glm::quat(1.0f, 0.f, 0.f, 0.f));
        }
        collision_resolution_.Resolve(collision_detection_.GetCollisions(), t, dt);
    }
    collision_detection_.Update(t, dt);
}

void PhysicsEngine::Draw(Shader *shader, const glm::mat4 &projection, const glm::mat4 &view) {
    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    auto &ecs = physics_world.ecs_;
    if (ecs != nullptr) {
        collision_detection_.Draw(shader, projection, view);
    }
}

PhysicsShape PhysicsEngine::CreateBoxShape(glm::vec3 extents) {
    return collision_detection_.CreateBoxShape(extents);
}

PhysicsShape PhysicsEngine::CreateCapsuleShape(double radius, double height) {
    return collision_detection_.CreateCapsuleShape(radius, height);
}

PhysicsShape PhysicsEngine::CreateSphereShape(double radius) {
    return collision_detection_.CreateSphereShape(radius);
}

bool PhysicsEngine::GetRendererStatus() {
    return collision_detection_.GetRendererStatus();
}

reactphysics3d::PhysicsWorld *PhysicsEngine::CreatePhysicsWorld() {
    return collision_detection_.CreatePhysicsWorld();
}

void PhysicsEngine::DestroyPhysicsWorld(reactphysics3d::PhysicsWorld * world) {
    collision_detection_.DeletePhysicsWorld(world);
}

void PhysicsEngine::Init() {
    collision_detection_.Init();
    //collision_resolution_.Init();
}
