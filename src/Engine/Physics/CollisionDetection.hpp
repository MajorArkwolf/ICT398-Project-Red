#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <queue>
#include "Engine/Physics/PhysicsData.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "PhysicsShape.hpp"

class ECS;

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

        /**
         * Toggles the collision renderer.
         */
        void ToggleRenderer();

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

        void Draw(const glm::mat4 &projection, const glm::mat4 &view);

        void Update(double t, double dt);

        physics::PhysicsShape CreateBoxShape(glm::vec3 extents);

        physics::PhysicsShape CreateCapsuleShape(double radius, double height);

        physics::PhysicsShape CreateSphereShape(double radius);


        int AddCollider(const entt::entity &entity_id, physics::PhysicsShape &shape, glm::vec3 relative_position,
                        glm::quat rotation);


        /// This que is used for handling collisions both detection and resolution.
        std::queue<PhysicsCollisionData> &GetCollisions();

        /// Gets the render status of the renderer.
        bool GetRendererStatus() const;

    private:

        /// Value to enable or disable renderering.
        bool renderer_ = false;

        //Test Renderer Values
        unsigned int l_vbo_ = 0, l_vao_ = 0;
        unsigned int t_vbo_ = 0, t_vao_ = 0;
        unsigned int line_num_ = 0, triag_num_ = 0;

        /// The shared pointer to draw react objects,
        std::unique_ptr<Shader> shader_ = nullptr;

        /// The react PhysicsCommon
        reactphysics3d::PhysicsCommon *physics_common_ = nullptr;

        /// The react PhysicsWorld
        reactphysics3d::PhysicsWorld *world_ = nullptr;

        /// The event lister in react to catch collission points.
        RedEngineEventListener event_listener_;

        /// Map lookup from an entity to a collission body.
        std::unordered_map<entt::entity, reactphysics3d::CollisionBody *> entity_collision_coupling_ = {};
        /// Map lookup from a collission body to an entity.
        std::unordered_map<reactphysics3d::CollisionBody *, entt::entity> collision_entity_coupling_ = {};

        /**
         * Creates a bidirectional relationship between entity and a collision body for fast lookup times.
         * @param entity the entity id
         * @param coll_body pointer to the react physics body
         */
        void AddBodyAndEntt(entt::entity &entity, reactphysics3d::CollisionBody *coll_body);
    };
}