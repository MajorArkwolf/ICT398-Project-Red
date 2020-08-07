#pragma once
#include <vector>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>

struct ContactPoints{
    glm::vec3 first_body_contact_point = {};
    glm::vec3 second_body_contact_point = {};
    glm::vec3 world_normal = {};
    float penetration = 0.0f;
};

struct PhysicsCollisionData {
    entt::entity first_body = entt::entity{0};
    entt::entity second_body = entt::entity{0};
    std::vector<ContactPoints> contact_points = {};
};