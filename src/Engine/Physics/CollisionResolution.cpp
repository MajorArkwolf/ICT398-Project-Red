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
    constexpr float restitution = 0.6f;
    auto& logger = redengine::Engine::get().GetLog();
    auto& first_transform = first_object->GetComponent<component::Transform>();
    auto& second_transform = second_object->GetComponent<component::Transform>();
    auto& first_physbody = first_object->GetComponent<component::PhysicBody>();
    auto& second_physbody = second_object->GetComponent<component::PhysicBody>();

    auto first_linear_momentum = first_physbody.mass * first_physbody.linear_velocity;
    auto second_linear_momentum = second_physbody.mass * second_physbody.linear_velocity;
    std::stringstream log_text;

    auto relative_velocity = second_physbody.linear_velocity - first_physbody.linear_velocity;
    for (auto& n : collision.contact_points) {
        /*log_text << "Body 1: {"
                 << n.first_body_contact_point.x << "," << n.first_body_contact_point.y << "," << n.first_body_contact_point.z << "}";
        log_text << " Body 2: {"
                 << n.second_body_contact_point.x << "," << n.second_body_contact_point.y << "," << n.second_body_contact_point.z << "}";
        logger.AddLog(ConsoleLog::LogType::Collision, log_text.str(), __LINE__, __FILE__);*/

        //auto impulse = n.collision_normal;
        //impulse *= -(1 + restitution) * first_physbody.mass * second_physbody.mass;
        //impulse *= glm::dot(n.collision_normal, second_physbody.linear_velocity - first_physbody.linear_velocity);
        //impulse /= (first_physbody.mass + second_physbody.mass);
        //first_physbody.linear_velocity -= impulse * first_physbody.inverse_mass;
        //second_physbody.linear_velocity += impulse * second_physbody.inverse_mass;
        auto relative_normal_velocity = glm::dot(relative_velocity, n.collision_normal);
        if (relative_normal_velocity >= 0) {
            auto impulse_scalar = (-1.0f + restitution) * relative_normal_velocity;
            impulse_scalar /= (first_physbody.mass + second_physbody.mass);
            auto impulse = impulse_scalar * n.collision_normal;
            first_physbody.linear_velocity -= (second_physbody.mass * impulse);
            second_physbody.linear_velocity += (first_physbody.mass * impulse);
        } else {

        }
    }
}
