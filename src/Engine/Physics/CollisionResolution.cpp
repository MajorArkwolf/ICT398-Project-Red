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
                ResolvePlayerCollision(item, ecs_->GetEntity(item.first_body), ecs_->GetEntity(item.second_body));
            } else {
                ResolvePhysicsCollision(item, ecs_->GetEntity(item.first_body), ecs_->GetEntity(item.second_body), dt);
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

void physics::CollisionResolution::ResolvePhysicsCollision(PhysicsCollisionData& collision, std::shared_ptr<Entity> first_object, std::shared_ptr<Entity> second_object, double dt) {
    constexpr float restitution = 0.6f;
    constexpr float slop = 0.01f;
    constexpr float beta = 0.2f;

    auto& logger = redengine::Engine::get().GetLog();

    //Get transforms and physics bodies
    auto& first_transform = first_object->GetComponent<component::Transform>();
    auto& second_transform = second_object->GetComponent<component::Transform>();
    auto& first_physbody = first_object->GetComponent<component::PhysicBody>();
    auto& second_physbody = second_object->GetComponent<component::PhysicBody>();

    //get velocities both angular and linear
    auto& lvelocity1 = first_physbody.linear_velocity;
    auto& wvelocity1 = first_physbody.angular_velocity;
    auto& lvelocity2 = second_physbody.linear_velocity;
    auto& wvelocity2 = second_physbody.angular_velocity;


    auto 
    std::stringstream log_text;

    auto relative_velocity = second_physbody.linear_velocity - first_physbody.linear_velocity;
    for (auto& n : collision.contact_points) {
        glm::vec3 deltaV = {lvelocity2 + glm::cross(wvelocity2, n.second_body_contact_point) - lvelocity1 - glm::cross(wvelocity1, n.first_body_contact_point)};
        float deltaVDotN = glm::dot(deltaV, n.collision_normal);
        float biasPenetrationDepth = 0.0;
        auto deltalambda = 0.f;
        if (n.penetration > slop) {
            biasPenetrationDepth = -(beta / dt) * std::max(0.0f, n.penetration - slop) + restitution;
        }
        float massPenetration = first_physbody.inverse_mass + second_physbody.inverse_mass 
            + ((mContactPoints[mNbContactPoints].i1TimesR1CrossN).cross(mContactPoints[mNbContactPoints].r1)).dot(mContactPoints[mNbContactPoints].normal) +
            ((mContactPoints[mNbContactPoints].i2TimesR2CrossN).cross(mContactPoints[mNbContactPoints].r2)).dot(mContactPoints[mNbContactPoints].normal);
        deltalambda = (deltaVDotN + biasPenetrationDepth)
        /*log_text << "Body 1: {"
                 << n.first_body_contact_point.x << "," << n.first_body_contact_point.y << "," << n.first_body_contact_point.z << "}";
        log_text << " Body 2: {"
                 << n.second_body_contact_point.x << "," << n.second_body_contact_point.y << "," << n.second_body_contact_point.z << "}";
        logger.AddLog(ConsoleLog::LogType::Collision, log_text.str(), __LINE__, __FILE__);*/

        //auto relative_normal_velocity = glm::dot(relative_velocity, n.collision_normal);
        //if (relative_normal_velocity > 1) {
        //    auto impulse_scalar = glm::dot(first_physbody.linear_velocity - second_physbody.linear_velocity, n.collision_normal);
        //    impulse_scalar *= -(1 + restitution) * first_physbody.mass * second_physbody.mass;
        //    impulse_scalar /= (first_physbody.mass + second_physbody.mass);
        //    auto impulse = impulse_scalar * n.collision_normal;

        //    first_physbody.linear_velocity += (impulse * first_physbody.inverse_mass) ;
        //    second_physbody.linear_velocity -= (impulse * second_physbody.inverse_mass) ;
        //    auto first_distance_to_com = (first_transform.pos + first_physbody.centre_mass) - n.first_body_contact_point;
        //    auto second_distance_to_com = (second_transform.pos + second_physbody.centre_mass) - n.second_body_contact_point;

        //    first_physbody.angular_velocity += (first_physbody.inverse_inertia_tensor * glm::cross(impulse, first_distance_to_com));
        //    second_physbody.angular_velocity += (second_physbody.inverse_inertia_tensor * glm::cross(impulse, second_distance_to_com));

        //    if (first_physbody.static_object) {
        //        second_physbody.should_apply_gravity = false;
        //    }
        //    if (second_physbody.static_object) {
        //        first_physbody.should_apply_gravity = false;
        //    }
        //    //const auto k_slop = 0.01f;// Penetration allowance
        //    //const auto percent = 0.01f;// Penetration percentage to correct
        //    //auto correction = (std::max(n.penetration - k_slop, 0.0f) / (first_physbody.mass + second_physbody.mass)) * percent * (n.collision_normal);
        //    //if (!first_physbody.static_object) {
        //    //    first_transform.pos += second_physbody.mass * correction;
        //    //}
        //    //if (!second_physbody.static_object) {
        //    //    second_transform.pos -= first_physbody.mass * correction;
        //    //}

        //} else {

        //}
    }
}
