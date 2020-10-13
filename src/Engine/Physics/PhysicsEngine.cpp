#include "PhysicsEngine.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Player.hpp"
#include "ECS/ECS.hpp"
#include "Engine/Engine.hpp"

using namespace physics;

void PhysicsEngine::FixedUpdate(double t, double dt) {
    collision_detection_.FixedUpdate(t, dt);
    IntegrateVelocities(dt);
    IntegratePositions(dt);
    collision_resolution_.Resolve(collision_detection_.GetCollisions(), t, dt);
    ResetAddedForces();

}

void PhysicsEngine::Update(double t, double dt) {
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

PhysicsShape PhysicsEngine::CreateCapsuleShape(float radius, float height) {
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

void PhysicsEngine::DestroyPhysicsWorld(reactphysics3d::PhysicsWorld *world) {
    collision_detection_.DeletePhysicsWorld(world);
}

void PhysicsEngine::Init() {
    collision_detection_.Init();
    //collision_resolution_.Init();
}

entt::entity PhysicsEngine::RayCastSingle(const glm::vec3 &start, const glm::vec3 &end) {
    return collision_detection_.RayCastSingle(start, end);
}

entt::entity PhysicsEngine::RayCastSingle(const glm::vec3 &start, const glm::vec3 &front, float distance) {
    glm::vec3 end = start;
    end += distance * front;
    return collision_detection_.RayCastSingle(start, end);
}

void physics::PhysicsEngine::IntegrateVelocities(double dt) {

    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    auto &ecs = physics_world.ecs_;
    if (ecs != nullptr) {
        auto &registry = ecs->GetRegistry();
        auto entities = registry.view<component::Transform, component::PhysicBody>();

        for (auto &e : entities) {
            auto &tran = entities.get<component::Transform>(e);
            auto &phys_body = entities.get<component::PhysicBody>(e);

            auto &linear_velocity = phys_body.linear_velocity;
            auto &angular_velocity = phys_body.angular_velocity;

            linear_velocity += float(dt) * (phys_body.inverse_mass * phys_body.added_force);
            angular_velocity += float(dt) * (phys_body.inertia_tensor * phys_body.added_torque);
        }

        if (physics_world.IsGravityEnabled()) {
            for (auto &e : entities) {
                auto &tran = entities.get<component::Transform>(e);
                auto &phys_body = entities.get<component::PhysicBody>(e);

                auto &linear_velocity = phys_body.linear_velocity;

                linear_velocity += float(dt) * (phys_body.inverse_mass * phys_body.mass * physics_world.GetGravity());
            }
        }
    }
}

void physics::PhysicsEngine::IntegratePositions(double dt) {
    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    auto &ecs = physics_world.ecs_;
    if (ecs != nullptr) {
        auto &registry = ecs->GetRegistry();
        auto entities = registry.view<component::Transform, component::PhysicBody>();

        for (auto &e : entities) {
            auto &tran = entities.get<component::Transform>(e);
            auto &phys_body = entities.get<component::PhysicBody>(e);

            auto &linear_velocity = phys_body.linear_velocity;
            auto &angular_velocity = phys_body.angular_velocity;
            auto &position = phys_body.position;
            auto &orientation = phys_body.orientation;

            position += linear_velocity * glm::vec3(dt);
            orientation += glm::quat(0.f, angular_velocity) * orientation * 0.5f * float(dt);

            tran.pos = phys_body.position;
            tran.rot = phys_body.orientation;
        }
    }
}

void physics::PhysicsEngine::ResetAddedForces() {
    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    auto &ecs = physics_world.ecs_;
    if (ecs != nullptr) {
        auto &registry = ecs->GetRegistry();
        auto entities = registry.view<component::Transform, component::PhysicBody>();

        for (auto &e : entities) {
            auto &tran = entities.get<component::Transform>(e);
            auto &phys_body = entities.get<component::PhysicBody>(e);

            phys_body.added_force = {0.f, 0.f, 0.f};
            phys_body.added_torque = {0.f, 0.f, 0.f};
        }
    }
}
