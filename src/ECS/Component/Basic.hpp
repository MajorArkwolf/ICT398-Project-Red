#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
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

    class PhysicBody {
       public:
        bool static_object = false;
        size_t id = 0;
        glm::dvec3 velocity = {};
        double mass = 0.0;
        glm::dvec3 centre_mass = {0, 0, 0};
        std::vector<redengine::Collider> colliders;
        //tensorflow mat3
        void AddForce(glm::dvec3 additional_force);
        void SetVelocity(glm::dvec3 new_velocity);
        glm::dvec3 GetVelocity() const;
    };
}
