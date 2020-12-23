#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "Engine/Physics/PhysicsData.hpp"
#include <queue>

namespace physics {
    class RedEngineEventListener : public reactphysics3d::EventListener {
    public:
        //Default Constructor
        RedEngineEventListener() = default;

        /**
         * Main constructor used.
         * @param c_e_c A map to look up collision bodies into entities.
         */
        RedEngineEventListener(std::unordered_map<reactphysics3d::CollisionBody *, entt::entity> *c_e_c);

        /**
         * Overloaded function that allows us to handle the collision detection
         * @param callbackData call back function pointer.
         */
        void onContact(const reactphysics3d::CollisionCallback::CallbackData &callbackData) override;

        /**
         * Detects when a trigger with a trigger body and collision body occures.
         * @param callbackData call back function pointer.
         */
        void onTrigger(const reactphysics3d::OverlapCallback::CallbackData &callbackData) override;

        /**
         * Gets the collision detection que.
         * @return all the collision points of data.
         */
        std::queue<PhysicsCollisionData> &GetPhysicsQueue();

        /**
         * Return all the triggers from the event listener.
         * @return a que of event triggers.
         */
        std::queue<PhysicsTriggerData>& GetPhysicsTriggerQueue();

        /// Used to look up entities from collision bodies.
        std::unordered_map<reactphysics3d::CollisionBody *, entt::entity> *collision_entity_coupling_ = nullptr;
        /// The que that will be used for handling entities.
        std::queue<PhysicsCollisionData> physics_que_ = {};
        /// The que that will handle all trigger collisions.
        std::queue<PhysicsTriggerData> physics_trigger_que_ = {};
    };

    class RedEngineRayCast : public reactphysics3d::RaycastCallback {

    public:
        struct redengineray {
            bool is_hit = false;
            reactphysics3d::CollisionBody* collisionBody = nullptr;
            reactphysics3d::Collider *collider = nullptr;
            glm::vec3 world_normal{};
            glm::vec3 world_point{};
        };
        float notifyRaycastHit(const reactphysics3d::RaycastInfo& info) override;
        redengineray result;
    };
}
