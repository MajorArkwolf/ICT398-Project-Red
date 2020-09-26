#pragma once

#include "Engine/Physics/CollisionDetection.hpp"
#include "Engine/Physics/CollisionResolution.hpp"

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

        PhysicsShape CreateCapsuleShape(double radius, double height);

        PhysicsShape CreateSphereShape(double radius);

        reactphysics3d::PhysicsWorld *CreatePhysicsWorld();

        void DestroyPhysicsWorld(reactphysics3d::PhysicsWorld *world);

        void Init();

        entt::entity RayCastSingle(const glm::vec3& start, const glm::vec3& end);

    private:
        CollisionDetection collision_detection_ = {};
        CollisionResolution collision_resolution_ = {};
    };
}
