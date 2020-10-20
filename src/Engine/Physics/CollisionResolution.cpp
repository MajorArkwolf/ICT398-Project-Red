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

    const auto& m1 = first_physbody.mass;
    const auto& m2 = second_physbody.mass;

    const auto& im1 = first_physbody.inverse_mass;
    const auto& im2 = second_physbody.inverse_mass;

    //first linear and angular vel
    auto& lv1 = first_physbody.linear_velocity;
    auto& wv1 = first_physbody.angular_velocity;

    //second object linear and angular vel
    auto& lv2 = second_physbody.linear_velocity;
    auto& wv2 = second_physbody.angular_velocity;

    //objects momentums
    auto p1 = first_physbody.mass * first_physbody.linear_velocity;
    auto p2 = second_physbody.mass * second_physbody.linear_velocity;


    std::stringstream log_text;

    for (auto& n : collision.contact_points) {
        auto com_to_col1 = (first_transform.pos + first_physbody.centre_mass) - n.first_body_contact_point;
        auto com_to_col2 = (second_transform.pos + second_physbody.centre_mass) - n.second_body_contact_point;
        /*log_text << "Body 1: {"
                 << n.first_body_contact_point.x << "," << n.first_body_contact_point.y << "," << n.first_body_contact_point.z << "}";
        log_text << " Body 2: {"
                 << n.second_body_contact_point.x << "," << n.second_body_contact_point.y << "," << n.second_body_contact_point.z << "}";
        logger.AddLog(ConsoleLog::LogType::Collision, log_text.str(), __LINE__, __FILE__);*/

        glm::vec3 rv = lv2 - lv1;
        auto cv = glm::dot(rv, n.collision_normal);


        auto j = (-1.0f + restitution) * cv;
        j /= (im1 + im2);
        auto impulse = j * n.collision_normal;
        lv1 -= im1 * impulse;
        lv2 += im2 * impulse;

         wv1 -= glm::cross(m1 * impulse, com_to_col1) * first_physbody.inverse_inertia_tensor;
         wv2 += glm::cross(com_to_col2, m2 * impulse) * second_physbody.inverse_inertia_tensor;
    }
}
