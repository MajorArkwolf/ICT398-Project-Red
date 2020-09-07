#include "PhysicsEngine.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Component/Basic.hpp"

using namespace physics;

void PhysicsEngine::FixedUpdate(double t, double dt) {
    collision_detection_.Update(t, dt);
}

void PhysicsEngine::Update(double t, double dt) {
    assert(ecs_ != nullptr);
    auto &registry = ecs_->GetRegistry();
    auto entities = registry.view<component::Transform, component::PhysicBody>();

    for (auto &e : entities) {
        auto &tran = entities.get<component::Transform>(e);
        collision_detection_.UpdateCollisionBody(e, tran.pos, tran.rot);
    }
    //collision_resolution_.Resolve(collision_detection_.GetCollisions(), t, dt);
}

void PhysicsEngine::Draw(const glm::mat4& projection, const glm::mat4& view, const glm::dvec2& cameraPos) {
    collision_detection_.Draw(projection, view);
}

void PhysicsEngine::SetECS(ECS *ecs) {
    this->ecs_ = ecs;
}

void PhysicsEngine::AddCollisionBody(const entt::entity &entity_id, const glm::vec3 &pos, const glm::quat &rot) {
    collision_detection_.AddCollisionBody(entity_id, pos, rot);
}

void PhysicsEngine::UpdateCollisionBody(const entt::entity &entity_id, const glm::vec3 &pos, const glm::quat &rot) {
    collision_detection_.UpdateCollisionBody(entity_id, pos, rot);
}

void PhysicsEngine::DeleteCollisionBody(const entt::entity &entity_id) {
    collision_detection_.DeleteCollisionBody(entity_id);
}

int PhysicsEngine::AddCollider(const entt::entity &entity_id, PhysicsShape &shape, glm::vec3 relative_position, glm::quat rotation) {
    return collision_detection_.AddCollider(entity_id, shape, relative_position, rotation);
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

void PhysicsEngine::ToggleRenderer() {
    collision_detection_.ToggleRenderer();
}

bool PhysicsEngine::GetRendererStatus() {
    return collision_detection_.GetRendererStatus();
}
