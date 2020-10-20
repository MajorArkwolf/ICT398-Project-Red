#pragma once
#include <glm/gtc/quaternion.hpp>
#include <queue>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include "DataStructures/Model/Animator.hpp"
#include "Engine/SubModules/PrefabRepo.hpp"

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
        float speed = 1.0f;
    };
    class PhysicBody {
    public:
        bool static_object = false;
        bool is_sleeping = false;
        size_t id = 0;
        //Primary
        glm::mat3x3 inertia_tensor = {};

        //Secondary
        glm::vec3 linear_velocity = {};
        glm::vec3 angular_velocity = {};
        glm::vec3 added_force = {};
        glm::vec3 added_torque = {};

        //Constants
        float mass = 0.0;
        float inverse_mass = 0.0;
        glm::vec3 centre_mass = {0, 0, 0};

        //Colliders
        std::vector<redengine::Collider> colliders;
        //tensorflow mat3

        void AddForce(glm::vec3 additional_force);
    };
}
// namespace component
