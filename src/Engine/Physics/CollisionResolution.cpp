#include "CollisionResolution.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Player.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Engine.hpp"
void physics::CollisionResolution::Resolve(std::queue<PhysicsCollisionData>& queue, double t, double dt) {
    ecs_ = redengine::Engine::get().game_stack_.getTop()->physics_world_.GetECS();
    while (!queue.empty()) {
        auto& item = queue.front();
        if (ecs_) {
            if (ecs_->GetRegistry().has<component::Player>(item.first_body)) {
                ResolvePlayerCollision(item, ecs_->GetEntity(item.first_body), ecs_->GetEntity(item.second_body));
            } else if (ecs_->GetRegistry().has<component::Player>(item.second_body)) {
                ResolvePlayerCollision(item, ecs_->GetEntity(item.second_body), ecs_->GetEntity(item.first_body));
            } else {
                ResolvePhysicsCollision(item, ecs_->GetEntity(item.second_body), ecs_->GetEntity(item.first_body));
            }
        }
        queue.pop();
    }
}

void physics::CollisionResolution::SetECS(ECS* ecs) {
    this->ecs_ = ecs;
}

void physics::CollisionResolution::ResolvePlayerCollision(PhysicsCollisionData& collision, std::shared_ptr<Entity> player, std::shared_ptr<Entity> other) {
}

void physics::CollisionResolution::ResolvePhysicsCollision(PhysicsCollisionData& collision, std::shared_ptr<Entity> first_object, std::shared_ptr<Entity> second_object) {
    auto& first_transform = first_object->GetComponent<component::Transform>();
    auto& second_transform = second_object->GetComponent<component::Transform>();
    auto& first_physbody = first_object->GetComponent<component::PhysicBody>();
    auto& second_physbody = second_object->GetComponent<component::PhysicBody>();

    auto relative_velocity = second_physbody.linear_velocity - first_physbody.linear_velocity;
    for (auto& n : collision.contact_points) {
        auto relative_normal_velocity = glm::dot(relative_velocity, n.world_normal);
        if (relative_normal_velocity >= 0) {
            auto impulse_scalar = (-1.0f) * relative_normal_velocity;
            impulse_scalar /= (first_physbody.mass + second_physbody.mass);
            auto impulse = impulse_scalar * n.world_normal;
            first_physbody.linear_velocity -= (second_physbody.mass * impulse);
            second_physbody.linear_velocity += (first_physbody.mass * impulse);
            const auto k_slop = 0.01f;// Penetration allowance
            const auto percent = 0.1f;// Penetration percentage to correct
            auto correction = (std::max(n.penetration - k_slop, 0.0f) / (first_physbody.mass + second_physbody.mass)) * percent * n.world_normal;
            first_transform.pos += first_physbody.mass * correction;
            second_transform.pos -= second_physbody.mass * correction;
        } else {

        }
    }
}
