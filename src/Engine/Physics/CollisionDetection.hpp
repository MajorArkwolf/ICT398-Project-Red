#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <queue>
#include "Engine/Physics/PhysicsData.hpp"
#include "Engine/Renderer/Shader.hpp"

class ECS;

class RedEngineEventListener : public reactphysics3d::EventListener {
public:
    RedEngineEventListener() = default;
    RedEngineEventListener(std::unordered_map<reactphysics3d::CollisionBody*, entt::entity>* c_e_c);
    void onContact(const reactphysics3d::CollisionCallback::CallbackData& callbackData) override;
    std::queue<PhysicsCollisionData>& GetPhysicsQueue ();
    std::unordered_map<reactphysics3d::CollisionBody*, entt::entity>* collision_entity_coupling_ = nullptr;
    std::queue<PhysicsCollisionData> physics_que_ = {};
};

class CollisionDetection {
public:
    /**
     * Default constructor, creates a physics world on construction.
     */
    CollisionDetection();
    ~CollisionDetection();
    void ToggleRenderer();
    void AddCollisionBody(const entt::entity& entity_id, const glm::vec3& pos, const glm::quat& rot);
    void UpdateCollisionBody(const entt::entity& entity_id, const glm::vec3& pos, const glm::quat& rot);
    void DeleteCollisionBody(const entt::entity& entity_id);
    void Draw(const glm::mat4& projection, const glm::mat4& view);
    void Update(double t, double dt);
    std::queue<PhysicsCollisionData>& GetCollisions();

private:
    bool renderer_ = false;

    //Test Renderer Values
    unsigned int l_vbo_ = 0, l_vao_ = 0;
    unsigned int t_vbo_ = 0, t_vao_ = 0;
    unsigned int line_num_ = 0, triag_num_ = 0;
    std::unique_ptr<Shader> line_shader_ = nullptr;
    std::unique_ptr<Shader> triangle_shader_ = nullptr;

    reactphysics3d::PhysicsCommon physics_common_{};
    reactphysics3d::PhysicsWorld* world_ = nullptr;
    RedEngineEventListener event_listener_;
    std::unordered_map<entt::entity, reactphysics3d::CollisionBody*> entity_collision_coupling_ = {};
    std::unordered_map<reactphysics3d::CollisionBody*, entt::entity> collision_entity_coupling_ = {};

    /**
     * Creates a relationship bidirectional relationship between entity and a collision body for fast lookup times.
     * @param entity the entity id
     * @param coll_body pointer to the react physics body
     */
    void AddBodyAndEntt(entt::entity& entity, reactphysics3d::CollisionBody* coll_body);
};
