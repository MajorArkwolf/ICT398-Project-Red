#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "DataStructures/Model/Animator.hpp"

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
        bool static_object = false;
    private:
        size_t id = 0;
        glm::dvec3 velocity = {};
        double mass = 1;
        //tensorflow mat3
    public:
        void AddForce(glm::dvec3 additional_force);
        void SetVelocity(glm::dvec3 new_velocity);
        glm::dvec3 GetVelocity() const;
    };
}
