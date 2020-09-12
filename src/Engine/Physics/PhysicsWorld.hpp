#pragma once

#include <reactphysics3d/engine/PhysicsWorld.h>
#include <entt/entt.hpp>
#include "Engine/Physics/PhysicsListener.hpp"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class ECS;

namespace physics {
    class PhysicsEngine;
    class CollisionDetection;


    class PhysicsWorld {
    public:
        friend PhysicsEngine;
        friend CollisionDetection;

        PhysicsWorld();

        explicit PhysicsWorld(ECS *ecs);

        ~PhysicsWorld();

        PhysicsWorld(const PhysicsWorld &) = delete;

        PhysicsWorld(PhysicsWorld &&) = default;

        PhysicsWorld &operator=(const PhysicsWorld &) = delete;

        PhysicsWorld &operator=(PhysicsWorld &&) = default;

        void SetECS(ECS *ecs);

        reactphysics3d::PhysicsWorld *GetWorld();

        /**
         * Adds a collision body to the detection.
         * @param entity_id the id of the entity
         * @param pos the position of said entity.
         * @param rot the rotation of said entity.
         */
        void AddCollisionBody(const entt::entity &entity_id, const glm::vec3 &pos, const glm::quat &rot);

        /**
         * Updates the collision body location
         * @param entity_id the id of the entity to update.
         * @param pos the current position of the entity.
         * @param rot the rotation of the current entity.
         */
        void UpdateCollisionBody(const entt::entity &entity_id, const glm::vec3 &pos, const glm::quat &rot);

        /**
         * Delete colission body both from the world and from the maps.
         * @param entity_id the entity id to drop.
         */
        void DeleteCollisionBody(const entt::entity &entity_id);

        std::queue<PhysicsCollisionData> &GetCollisions();

    private:
        bool renderer_ = false;
        ECS *ecs_ = nullptr;
        /// The react PhysicsWorld
        reactphysics3d::PhysicsWorld *world_ = nullptr;
        /// Map lookup from an entity to a collission body.
        std::unordered_map<entt::entity, reactphysics3d::CollisionBody *> entity_collision_coupling_ = {};
        /// Map lookup from a collission body to an entity.
        std::unordered_map<reactphysics3d::CollisionBody *, entt::entity> collision_entity_coupling_ = {};
        RedEngineEventListener event_listener_;

        void AddBodyAndEntt(entt::entity &entity, reactphysics3d::CollisionBody *coll_body);
    };
}

