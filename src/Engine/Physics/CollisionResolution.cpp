﻿#include "CollisionResolution.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Player.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Engine.hpp"
void physics::CollisionResolution::Resolve(std::queue<PhysicsCollisionData> &queue, double t, double dt) {
    ecs_ = redengine::Engine::get().game_stack_.getTop()->physics_world_.GetECS();
    while (!queue.empty()) {
        auto &item = queue.front();
        if (ecs_) {
            ResolvePhysicsCollision(item, ecs_->GetEntity(item.first_body), ecs_->GetEntity(item.second_body));

        }
        queue.pop();
    }
}

void physics::CollisionResolution::SetECS(ECS *ecs) {
    this->ecs_ = ecs;
}

void
physics::CollisionResolution::ResolvePlayerCollision(PhysicsCollisionData &collision, std::shared_ptr<Entity> player,
                                                     std::shared_ptr<Entity> other) {
}

void physics::CollisionResolution::ResolvePhysicsCollision(PhysicsCollisionData &collision,
                                                           std::shared_ptr<Entity> first_object,
                                                           std::shared_ptr<Entity> second_object) {
    constexpr float restitution = 0.6f;
    auto &logger = redengine::Engine::get().GetLog();
    auto &first_transform = first_object->GetComponent<component::Transform>();
    auto &second_transform = second_object->GetComponent<component::Transform>();
    auto &first_physbody = first_object->GetComponent<component::PhysicBody>();
    auto &second_physbody = second_object->GetComponent<component::PhysicBody>();

    auto lvelocity1 = first_physbody.linear_velocity;
    auto wvelocity1 = first_physbody.angular_velocity;
    auto lvelocity2 = second_physbody.linear_velocity;
    auto wvelocity2 = second_physbody.angular_velocity;

    std::stringstream log_text;

    for (auto &n : collision.contact_points) {
        if ((first_physbody.is_sleeping || first_physbody.static_object) && (second_physbody.is_sleeping || second_physbody.static_object)) {
            continue;
        }

        /*log_text << "Body 1: {"
                 << n.first_body_contact_point.x << "," << n.first_body_contact_point.y << "," << n.first_body_contact_point.z << "}";
        log_text << " Body 2: {"
                 << n.second_body_contact_point.x << "," << n.second_body_contact_point.y << "," << n.second_body_contact_point.z << "}";
        logger.AddLog(ConsoleLog::LogType::Collision, log_text.str(), __LINE__, __FILE__);*/

        glm::vec3 r1 = n.first_body_contact_point - (first_transform.pos + first_physbody.centre_mass);
        glm::vec3 r2 = n.second_body_contact_point - (second_transform.pos + second_physbody.centre_mass);

        if (!first_physbody.is_sleeping && !first_physbody.static_object) {
            first_transform.pos += n.contact_normal * ((n.penetration / 2) * -1);
        }
        if (!second_physbody.is_sleeping && !second_physbody.static_object) {
            second_transform.pos -= n.contact_normal * ((n.penetration / 2) * -1);
        }

        //Transfer of momentum

        //         -(1 + ε) * (n̂ • (v⁻₁ - v⁻₂) + w⁻₁ • (r₁ x n̂) - w₂ • (r₂ x n̂))
        // __________________________________________________________________________ * n̂
        // (m₁⁻¹ + m₂⁻¹) + ((r₁ x n̂)ᵀ J₁⁻¹ * (r₁ x n̂) + (r₂ x n̂)ᵀ * J₂⁻¹ * (r₂ x n̂)

        //  -(1 + ε)
        auto restitution_multiplier = -(1.0f + restitution);

        // (v⁻₁ - v⁻₂)
        auto relative_velocity = lvelocity1 - lvelocity2;

        //(r₁ x n̂)
        auto r1xn = glm::cross(r1, n.contact_normal);

        //(r₂ x n̂)
        auto r2xn = glm::cross(r2, n.contact_normal);

        // (m₁⁻¹ + m₂⁻¹)
        auto total_inverse_mass = first_physbody.inverse_mass + second_physbody.inverse_mass;

        //-(1 + ε) * (n̂ • (v⁻₁ - v⁻₂) + w⁻₁ • (r₁ x n̂) - w₂ • (r₂ x n̂))
        auto numerator = restitution_multiplier * (glm::dot(n.contact_normal, relative_velocity) + glm::dot(wvelocity1, r1xn) - glm::dot(wvelocity2, r2xn));

        // (m₁⁻¹ + m₂⁻¹) + ((r₁ x n̂)ᵀ * J₁⁻¹ * (r₁ x n̂) + (r₂ x n̂)ᵀ * J₂⁻¹ * (r₂ x n̂)
        float denominator = total_inverse_mass + (glm::dot(r1xn, first_physbody.inverse_inertia_tensor * r1xn) + glm::dot(r2xn, second_physbody.inverse_inertia_tensor * r2xn));

        //         -(1 + ε) * (n̂ • (v⁻₁ - v⁻₂) + w⁻₁ • (r₁ x n̂) - w₂ • (r₂ x n̂))
        // __________________________________________________________________________ * n̂
        // (m₁⁻¹ + m₂⁻¹) + ((r₁ x n̂)ᵀ * J₁⁻¹ * (r₁ x n̂) + (r₂ x n̂)ᵀ * J₂⁻¹ * (r₂ x n̂)

        //Transfer of momentum
        auto lambda = (numerator / denominator);

        //linear impulse
        auto linear_impulse = lambda * n.contact_normal;

        if (lambda < 0) {
            // v⁺₁ = v⁻₁
            lvelocity1 += linear_impulse * first_physbody.inverse_mass;
            // v⁺₂ = v⁻₂
            lvelocity2 -= linear_impulse * second_physbody.inverse_mass;

            wvelocity1 = wvelocity1 + (lambda * first_physbody.inverse_inertia_tensor) * r1xn;
            wvelocity2 = wvelocity2 - (lambda * second_physbody.inverse_inertia_tensor) * r2xn;
        }

    }
    if (!first_physbody.is_player) {
        first_physbody.linear_velocity = lvelocity1;
        first_physbody.angular_velocity = wvelocity1;
    }
    if (!second_physbody.is_player) {
        second_physbody.linear_velocity = lvelocity2;
        second_physbody.angular_velocity = wvelocity2;
    }
}
