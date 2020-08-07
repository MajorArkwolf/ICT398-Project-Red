#include "PhysicsEngine.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Component/Basic.hpp"

void PhysicsEngine::FixedUpdate(double t, double dt) {
    assert(ecs_ != nullptr);
    auto &registry = ecs_->GetRegistry();
    auto entities = registry.view<component::Transform, component::PhysicBody>();

    for (auto &e : entities) {
        auto &tran = entities.get<component::Transform>(e);
        collision_detection_.UpdateCollisionBody(e, tran.pos, tran.rot);
    }

}

void PhysicsEngine::Update(double t, double dt) {

}

void PhysicsEngine::Draw(const glm::mat4& projection, const glm::mat4& view, const glm::dvec2& cameraPos) {

}

void PhysicsEngine::SetECS(ECS *ecs) {
    this->ecs_ = ecs;
}
