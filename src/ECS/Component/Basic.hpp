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

        struct State {
            //primary
            glm::vec3 position = {};
            glm::vec3 momentum = {};

            //Secondary
            glm::vec3 velocity = {};

            //constant 
            float mass = 0;
            float inverseMass = 0;
            void Recalculate();
        } state;

        struct Derivative {
            glm::vec3 velocity = {};
            glm::vec3 force = {};
        } derivative;


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
