#pragma once
#include <entt/entt.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <queue>

#include "DataStructures/Model/Animator.hpp"
#include "Engine/SubModules/PrefabRepo.hpp"
#include "Node.hpp"

namespace component {
struct Transform {
    glm::vec3 pos = {0.0f, 0.0f, 0.0f};
    glm::quat rot = {1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
};

struct Animation {
    explicit Animation(size_t modelID) {
        animator_.LinkToModel(modelID);
    }
    ::model::Animator animator_ = {};
};

struct Moving {
    glm::vec3 going_to = glm::vec3{0.f, 0.f, 0.f};
    bool is_moving = false;
    std::queue<entt::entity> move_list{};
    entt::entity last_node;
    entt::entity going_to_node;
    void SetLastNode(entt::registry& reg, entt::entity id);
    void SetGoingToNode(entt::registry& reg, entt::entity id);
    float speed = 1.0f;
};

class PhysicBody {
   public:
    bool static_object = false;
    bool is_sleeping = false;
    bool should_apply_gravity = true;
    bool is_player = false;

    size_t id = 0;

    glm::mat3x3 inertia_tensor = {};
    glm::mat3x3 inverse_inertia_tensor = {};
    glm::vec3 linear_velocity = {0, 0, 0};
    glm::vec3 angular_velocity = {0, 0, 0};
    glm::vec3 added_force = {0, 0, 0};
    glm::vec3 added_torque = {0, 0, 0};

    //Constants
    float mass = 0.0;
    float inverse_mass = 0.0;
    glm::vec3 centre_mass = {0, 0, 0};

    //Colliders
    std::vector<redengine::Collider> colliders;

    void AddForce(glm::vec3 additional_force);
    void AddTorque(glm::vec3 additional_force);
};

struct NPCPersonalityID {
    int ID = 0;
};

struct InteractableObject {
    enum class Type {
        unknown,
        npc,
        book,
        tree,
        bench,
        rock,
    };
    Type type = Type::unknown;
};

}// namespace component

