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
                ResolvePhysicsCollision(item, ecs_->GetEntity(item.first_body), ecs_->GetEntity(item.second_body));
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

    auto& lvelocity1 = first_physbody.linear_velocity;
    auto& wvelocity1 = first_physbody.angular_velocity;
    auto& lvelocity2 = second_physbody.linear_velocity;
    auto& wvelocity2 = second_physbody.angular_velocity;

    std::stringstream log_text;

    for (auto& n : collision.contact_points) {
        /*log_text << "Body 1: {"
                 << n.first_body_contact_point.x << "," << n.first_body_contact_point.y << "," << n.first_body_contact_point.z << "}";
        log_text << " Body 2: {"
                 << n.second_body_contact_point.x << "," << n.second_body_contact_point.y << "," << n.second_body_contact_point.z << "}";
        logger.AddLog(ConsoleLog::LogType::Collision, log_text.str(), __LINE__, __FILE__);*/

        glm::vec3 com_to_col1 = n.first_body_contact_point - (first_transform.pos + first_physbody.centre_mass);
        glm::vec3 com_to_col2 = n.second_body_contact_point - (second_transform.pos + second_physbody.centre_mass);

        //         -(1 + ε) * (n̂ • (v⁻₁ - v⁻₂) + w⁻₁ • (r₁ x n̂) - w₂ • (r₂ x n̂))
        // __________________________________________________________________________ * n̂
        // (m₁⁻¹ + m₂⁻¹) + ((r₁ x n̂)ᵀ J₁⁻¹ * (r₁ x n̂) + (r₂ x n̂)ᵀ * J₂⁻¹ * (r₂ x n̂)

        //  -(1 + ε)
        auto restitution_multiplier = -(1.0f + restitution);

        // (v⁻₁ - v⁻₂)
        auto relative_velocity = lvelocity1 - lvelocity2;

        //(r₁ x n̂)
        auto r1xn = glm::cross(com_to_col1, n.collision_normal);

        //(r₂ x n̂)
        auto r2xn = glm::cross(com_to_col2, n.collision_normal);

        // (m₁⁻¹ + m₂⁻¹)
        auto total_inverse_mass = first_physbody.inverse_mass + second_physbody.inverse_mass;

        //-(1 + ε) * (n̂ • (v⁻₁ - v⁻₂) + w⁻₁ • (r₁ x n̂) - w₂ • (r₂ x n̂))
        float numerator = restitution_multiplier * (glm::dot(n.collision_normal, relative_velocity) + glm::dot(wvelocity1, r1xn) - glm::dot(wvelocity2, r2xn));

        // (m₁⁻¹ + m₂⁻¹) + ((r₁ x n̂)ᵀ * J₁⁻¹ * (r₁ x n̂) + (r₂ x n̂)ᵀ * J₂⁻¹ * (r₂ x n̂)
        float denominator = total_inverse_mass + glm::dot(r1xn, first_physbody.inverse_inertia_tensor * r1xn) + glm::dot(r2xn, second_physbody.inverse_inertia_tensor * r2xn);

        //         -(1 + ε) * (n̂ • (v⁻₁ - v⁻₂) + w⁻₁ • (r₁ x n̂) - w₂ • (r₂ x n̂))
        // __________________________________________________________________________ * n̂
        // (m₁⁻¹ + m₂⁻¹) + ((r₁ x n̂)ᵀ * J₁⁻¹ * (r₁ x n̂) + (r₂ x n̂)ᵀ * J₂⁻¹ * (r₂ x n̂)
        auto impulse = (numerator / denominator) * n.collision_normal;

        lvelocity1 += impulse * first_physbody.inverse_mass;
        lvelocity2 -= impulse * second_physbody.inverse_mass;

        wvelocity1 += glm::length(impulse) * first_physbody.inverse_inertia_tensor * r1xn;
        wvelocity2 -= glm::length(impulse) * second_physbody.inverse_inertia_tensor * r2xn;
    }
}
