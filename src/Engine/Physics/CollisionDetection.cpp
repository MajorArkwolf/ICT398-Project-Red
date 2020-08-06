#include "CollisionDetection.hpp"
#include "ECS/Ecs.hpp"

static inline glm::vec3 ConvertPosition(const reactphysics3d::Vector3& r_vec) {
    glm::vec3 pos = {};
    pos.x = r_vec.x;
    pos.y = r_vec.y;
    pos.z = r_vec.z;
    return pos;
}

void RedEngineEventListener::onContact(const reactphysics3d::CollisionCallback::CallbackData &callbackData) {
    using namespace reactphysics3d;
    // For each contact pair
    for (uint p = 0; p < callbackData.getNbContactPairs(); p++) {

        // Get the contact pair
        CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);

        // For each contact point of the contact pair
        for (uint c = 0; c < contactPair.getNbContactPoints(); c++) {

            // Get the contact point
            CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c);

            // Get the contact point on the first collider and convert it in world-space
            Vector3 worldPoint = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
        }
    }
}

static inline reactphysics3d::Vector3 ConvertPosition(const glm::vec3& glm_vec) {
    auto pos = reactphysics3d::Vector3();
    pos.x = glm_vec.x;
    pos.y = glm_vec.y;
    pos.z = glm_vec.z;
    return pos;
}

static inline reactphysics3d::Quaternion ConvertRotation(const glm::quat& glm_quat) {
    auto rot = reactphysics3d::Quaternion();
    rot.w = glm_quat.w;
    rot.x = glm_quat.x;
    rot.y = glm_quat.y;
    rot.z = glm_quat.z;
    return rot;
}

CollisionDetection::CollisionDetection() {
    world_ = physics_common_.createPhysicsWorld();
    world_->setEventListener(&event_listener);
}

CollisionDetection::~CollisionDetection() {
    physics_common_.destroyPhysicsWorld(world_);
}

void CollisionDetection::AddCollisionBody(const entt::entity& entity_id, const glm::vec3& pos, const glm::quat& rot) {
    reactphysics3d::Transform transform = {};
    transform.setPosition(ConvertPosition(pos));
    transform.setOrientation(ConvertRotation(rot));
    reactphysics3d::CollisionBody* body = world_->createCollisionBody(transform);
    AddBodyAndEntt(const_cast<entt::entity &>(entity_id), body);
}


void CollisionDetection::AddBodyAndEntt(entt::entity &entity, reactphysics3d::CollisionBody* coll_body) {
    auto r = std::make_pair(entity, coll_body);
    auto y = std::make_pair(coll_body, entity);
    entity_collision_coupling_.insert(r);
    collision_entity_coupling_.insert(y);
}

void CollisionDetection::UpdateCollisionBody(const entt::entity &entity_id, const glm::vec3& pos, const glm::quat& rot) {
    auto body = entity_collision_coupling_.at(entity_id);
    reactphysics3d::Transform new_transform(ConvertPosition(pos), ConvertRotation(rot));
    body->setTransform(new_transform);
}

void CollisionDetection::DeleteCollisionBody(const entt::entity &entity_id) {
    auto body = entity_collision_coupling_.at(entity_id);
    entity_collision_coupling_.erase(entity_id);
    collision_entity_coupling_.erase(body);
    world_->destroyCollisionBody(body);
}
