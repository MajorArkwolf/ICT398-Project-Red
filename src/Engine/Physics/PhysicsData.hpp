#pragma once

#include <vector>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>

/**
 * The contact point of where two entities hit.
 */
struct ContactPoints{
    glm::vec3 first_body_contact_point = {};
    glm::vec3 second_body_contact_point = {};
    glm::vec3 contact_normal = {};
    float penetration = 0.0f;
};

/**
 * This holds the ID and the contact points between two bodies.
 */
struct PhysicsCollisionData {
    entt::entity first_body = entt::entity{0};
    entt::entity second_body = entt::entity{0};
    std::vector<ContactPoints> contact_points = {};
};

struct PhysicsTriggerData {
    enum class Event {
        start,
        exit,
        stay
    };
    entt::entity first_body = entt::entity{0};
    entt::entity second_body = entt::entity{0};
    Event trigger_event = {};
};
