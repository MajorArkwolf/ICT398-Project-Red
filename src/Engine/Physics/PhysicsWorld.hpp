#pragma once

#include <reactphysics3d/engine/PhysicsWorld.h>
#include <entt/entt.hpp>

class ECS;

namespace physics {
    class PhysicsEngine;


    class PhysicsWorld {
        friend PhysicsEngine;

        PhysicsWorld();

        ~PhysicsWorld() = default;

        void SetECS(ECS *ecs);

    private:
        ECS *ecs_ = nullptr;
        /// The react PhysicsWorld
        reactphysics3d::PhysicsWorld *world_ = nullptr;
        /// Map lookup from an entity to a collission body.
        std::unordered_map<entt::entity, reactphysics3d::CollisionBody *> entity_collision_coupling_ = {};
        /// Map lookup from a collission body to an entity.
        std::unordered_map<reactphysics3d::CollisionBody *, entt::entity> collision_entity_coupling_ = {};
    };
}

