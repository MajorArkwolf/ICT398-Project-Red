#pragma once
#include "Engine/Physics/CollisionDetection.hpp"
#include "Engine/Physics/CollisionResolution.hpp"

class ECS;

namespace physics {

    class PhysicsEngine {
    public:
        PhysicsEngine() = default;

        ~PhysicsEngine() = default;

        PhysicsEngine(PhysicsEngine &p) = delete;

        PhysicsEngine(PhysicsEngine &&p) = delete;

        PhysicsEngine &operator=(PhysicsEngine p) = delete;

        PhysicsEngine &operator=(PhysicsEngine &&p) = delete;

        void FixedUpdate(double t, double dt);

        void Update(double t, double dt);

        void Draw(const glm::mat4 &projection, const glm::mat4 &view, const glm::dvec2 &cameraPos);

        void ToggleRenderer();

        bool GetRendererStatus();

        void SetECS(ECS *ecs);

        int AddCollider(const entt::entity& entity_id, PhysicsShape& shape, glm::vec3 relative_position, glm::quat rotation);

        PhysicsShape CreateBoxShape(glm::vec3 extents);

        PhysicsShape CreateCapsuleShape(double radius, double height);

        PhysicsShape CreateSphereShape(double radius);

    private:
        ECS *ecs_ = nullptr;
        CollisionDetection collision_detection_ = {};
        CollisionResolution collision_resolution_ = {};
    };
}
