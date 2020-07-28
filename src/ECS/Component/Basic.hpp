#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "DataStructures/Model/Animator.hpp"

namespace Component {
    struct Transform {
        glm::vec3 pos = {0.0f, 0.0f, 0.0f};
        glm::quat rot = {1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 scale = {1.0f, 1.0f, 1.0f};
    };

    struct Animation {
        explicit Animation(size_t modelID) {
            animator.LinkToModel(modelID);
        }
        ::Model::Animator animator = {};
    };
}
