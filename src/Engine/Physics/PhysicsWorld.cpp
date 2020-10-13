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

void PhysicsWorld::AddBodyAndEntt(entt::entity entity, reactphysics3d::CollisionBody* coll_body) {
    entity_collision_coupling_.insert({entity, coll_body});
    collision_entity_coupling_.insert({coll_body, entity});
}

void PhysicsWorld::AddCollisionBody(const entt::entity& entity_id, const glm::vec3& pos, const glm::quat& rot) {
    reactphysics3d::Transform transform = {};
    transform.setPosition(ConvertVector(pos));
    transform.setOrientation(ConvertQuaternion(rot));
    auto* body = world_->createCollisionBody(transform);
    AddBodyAndEntt(entity_id, body);
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

unsigned int PhysicsWorld::AddCollider(const entt::entity& entity_id, PhysicsShape& shape, glm::vec3 relative_position, glm::quat rotation) {
    auto* body = this->entity_collision_coupling_.at(entity_id);
    rp3d::Transform transform(ConvertVector(relative_position), ConvertQuaternion(rotation));
    body->addCollider(shape.shape_, transform);
    return body->getNbColliders();
}

void PhysicsWorld::ToggleRenderer() {
    this->renderer_ = !this->renderer_;
    this->world_->setIsDebugRenderingEnabled(this->renderer_);
    // Get a reference to the debug renderer
    reactphysics3d::DebugRenderer& debug_renderer = this->world_->getDebugRenderer();
    if (this->renderer_) {
        // Select the contact points and contact normals to be displayed
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
    } else {
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, false);
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, false);
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, false);
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_AABB, false);
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, false);
    }
}

bool physics::PhysicsWorld::IsGravityEnabled() {
    return gravity_enabled;
}

void physics::PhysicsWorld::SetGravityEnabled(bool enabled) {
    gravity_enabled = enabled;
}

const glm::vec3& physics::PhysicsWorld::GetGravity() {
    return gravity;
}

void physics::PhysicsWorld::SetGravity(const glm::vec3& gravity_) {
    gravity = gravity_;
}

ECS* physics::PhysicsWorld::GetECS() {
    return ecs_;
}
