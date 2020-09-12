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
         * Gets the collision detection que.
         * @return
         */
        std::queue<PhysicsCollisionData> &GetPhysicsQueue();

        /// Used to look up entities from collision bodies.
        std::unordered_map<reactphysics3d::CollisionBody *, entt::entity> *collision_entity_coupling_ = nullptr;
        /// The que that will be used for handling entities.
        std::queue<PhysicsCollisionData> physics_que_ = {};
    };

}
