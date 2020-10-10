#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <queue>

#include "Engine/Renderer/Shader.hpp"
#include "PhysicsShape.hpp"
#include "Logger.hpp"
#include "PhysicsData.hpp"

class ECS;

namespace physics {

    class CollisionDetection {
    public:
        /**
         * Default constructor, creates a physics world on construction.
         */
        CollisionDetection();

        /**
         * Default Destructor.
         */
        ~CollisionDetection();

        void Init();

        void Draw(Shader *shader, const glm::mat4 &projection, const glm::mat4 &view);

        void FixedUpdate(double t, double dt);

        void Update(double t, double dt);

        PhysicsShape CreateBoxShape(glm::vec3 extents);

        PhysicsShape CreateCapsuleShape(double radius, double height);

        PhysicsShape CreateSphereShape(double radius);


        /// This que is used for handling collisions both detection and resolution.
        std::queue<PhysicsCollisionData> &GetCollisions();

        /// Gets the render status of the renderer.
        bool GetRendererStatus() const;

        /**
         * Creates a new physics world
         * @return pointer handled by react.
         */
        reactphysics3d::PhysicsWorld *CreatePhysicsWorld();

        /**
         * Destroys the physics world at the given address.
         * @param world to be destroyed.
         */
        void DeletePhysicsWorld(reactphysics3d::PhysicsWorld *world);

        /**
         * Cast a single ray from one point to another.
         * @param start the start vector.
         * @param end end of the ray.
         * @return the single entity you hit.
         */
        entt::entity RayCastSingle(const glm::vec3 &start, const glm::vec3 &end);

    private:

        //Test Renderer Values
        unsigned int l_vbo_ = 0, l_vao_ = 0;
        unsigned int t_vbo_ = 0, t_vao_ = 0;
        unsigned int line_num_ = 0, triag_num_ = 0;

        /// The shared pointer to draw react objects,
        std::unique_ptr<Shader> shader_ = nullptr;

        /// The react PhysicsCommon
        reactphysics3d::PhysicsCommon physics_common_{};

        Logger logger_ = {};

    };
}
