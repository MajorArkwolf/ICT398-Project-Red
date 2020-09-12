#include "PhysicsWorld.hpp"
#include "Engine/Engine.hpp"

using namespace physics;

static inline glm::vec3 ConvertVector(const reactphysics3d::Vector3& r_vec) {
    glm::vec3 pos = {};
    pos.x = r_vec.x;
    pos.y = r_vec.y;
    pos.z = r_vec.z;
    return pos;
}

static inline reactphysics3d::Vector3 ConvertVector(const glm::vec3& glm_vec) {
    auto pos = reactphysics3d::Vector3();
    pos.x = glm_vec.x;
    pos.y = glm_vec.y;
    pos.z = glm_vec.z;
    return pos;
}

static inline reactphysics3d::Quaternion ConvertQuaternion(const glm::quat& glm_quat) {
    auto rot = reactphysics3d::Quaternion();
    rot.w = glm_quat.w;
    rot.x = glm_quat.x;
    rot.y = glm_quat.y;
    rot.z = glm_quat.z;
    return rot;
}

PhysicsWorld::PhysicsWorld() {
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    world_ = physics_engine.CreatePhysicsWorld();
    event_listener_ = RedEngineEventListener(&collision_entity_coupling_);
    world_->setEventListener(&event_listener_);
}

void PhysicsWorld::SetECS(ECS *ecs) {
    this->ecs_ = ecs;
}

PhysicsWorld::PhysicsWorld(ECS *ecs) {
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    world_ = physics_engine.CreatePhysicsWorld();
    event_listener_ = RedEngineEventListener(&collision_entity_coupling_);
    world_->setEventListener(&event_listener_);
    ecs_ = ecs;
}

PhysicsWorld::~PhysicsWorld() {
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    physics_engine.DestroyPhysicsWorld(world_);
}

reactphysics3d::PhysicsWorld *PhysicsWorld::GetWorld() {
    return world_;
}

void PhysicsWorld::AddBodyAndEntt(entt::entity& entity, reactphysics3d::CollisionBody* coll_body) {
    auto r = std::make_pair(entity, coll_body);
    auto y = std::make_pair(coll_body, entity);
    entity_collision_coupling_.emplace(r);
    collision_entity_coupling_.emplace(y);
}

void PhysicsWorld::AddCollisionBody(const entt::entity& entity_id, const glm::vec3& pos, const glm::quat& rot) {
    reactphysics3d::Transform transform = {};
    transform.setPosition(ConvertVector(pos));
    transform.setOrientation(ConvertQuaternion(rot));
    auto* body = world_->createCollisionBody(transform);
    AddBodyAndEntt(const_cast<entt::entity&>(entity_id), body);
}

void PhysicsWorld::UpdateCollisionBody(const entt::entity &entity_id, const glm::vec3& pos, const glm::quat& rot) {
    if (entity_collision_coupling_.find(entity_id) != entity_collision_coupling_.end()) {
        auto body = entity_collision_coupling_.at(entity_id);
        reactphysics3d::Transform new_transform(ConvertVector(pos), ConvertQuaternion(rot));
        body->setTransform(new_transform);
    }
}

void PhysicsWorld::DeleteCollisionBody(const entt::entity& entity_id) {
    auto body = entity_collision_coupling_.at(entity_id);
    entity_collision_coupling_.erase(entity_id);
    collision_entity_coupling_.erase(body);
    world_->destroyCollisionBody(body);
}

std::queue<PhysicsCollisionData>& PhysicsWorld::GetCollisions() {
    return event_listener_.GetPhysicsQueue();
}

