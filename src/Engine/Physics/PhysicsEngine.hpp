#pragma once

#include "Engine/Physics/CollisionDetection.hpp"
#include "Engine/Physics/CollisionResolution.hpp"
#include <Engine\SubModules\PrefabRepo.hpp>

class Shader;

namespace physics {

    class PhysicsEngine {
    public:
        /**
         * Default constructor.
         */
        PhysicsEngine() = default;

        /**
         * Default destructor.
         */
        ~PhysicsEngine() = default;

        /**
         * Deleted copy constructor.
         * @param p
         */
        PhysicsEngine(PhysicsEngine &p) = delete;

        /**
         * Deleted move constructor
         * @param p
         */
        PhysicsEngine(PhysicsEngine &&p) = delete;

        /**
         * Deleted copy assignment.
         * @param p
         * @return
         */
        PhysicsEngine &operator=(PhysicsEngine p) = delete;

        /**
         * Deleted move assignment.
         * @param p
         * @return
         */
        PhysicsEngine &operator=(PhysicsEngine &&p) = delete;

        /**
         * Fixed Update, commonly used to update physics based components.
         * @param t time since engine started.
         * @param dt time since last frame aka delta time.
         */
        void FixedUpdate(double t, double dt);

        /**
         * This is used for non physics related tasks such as setting up the display renderer for the draw call.
         * @param t time since engine started.
         * @param dt time since last frame aka delta time.
         */
        void Update(double t, double dt);


        /**
         * Draws the physics world over the real world.
         * @param projection projection matrix of the camera.
         * @param view the view matrix of the camera.
         */
        void Draw(Shader *shader, const glm::mat4 &projection, const glm::mat4 &view);

        /**
         * Returns if the renderer is on.
         * @return true if renderer is on.
         */
        bool GetRendererStatus();

        PhysicsShape CreateBoxShape(glm::vec3 extents);

        PhysicsShape CreateCapsuleShape(float radius, float height);

        PhysicsShape CreateSphereShape(double radius);

        reactphysics3d::PhysicsWorld *CreatePhysicsWorld();

        void DestroyPhysicsWorld(reactphysics3d::PhysicsWorld *world);
        glm::mat3x3 CalculateInertiaTensor(redengine::Box shape, float mass);
        glm::mat3x3 CalculateInertiaTensor(redengine::Capsule shape, float mass);
        glm::mat3x3 CalculateInertiaTensor(redengine::Sphere shape, float mass);

        void Init();

        /**
         * Ray cast in a given direction to determine if a object is hit.
         * @param start the start vector.
         * @param end the end vector.
         * @return the entity id if hit or -1 if not hit.
         */
        entt::entity RayCastSingle(const glm::vec3& start, const glm::vec3& end);

        /**
         * Ray cast in a given direction to determine if a object is hit.
         * @param start the start location of the object.
         * @param front the front vector of the object.
         * @param distance the distance in which to ray cast before giving up.
         * @return the entity id if hit or -1 if not hit.
         */
        entt::entity RayCastSingle(const glm::vec3 &start, const glm::vec3 &front, float distance);

        void SetTrigger(entt::entity entity, bool is_trigger);
        void IntegrateVelocities(double dt);
        void IntegratePositions(double dt);

    private:
        CollisionDetection collision_detection_ = {};
        CollisionResolution collision_resolution_ = {};

        void ResetAddedForces();
    };
}
