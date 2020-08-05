#include "CollisionDetection.hpp"
#include "ECS/Ecs.hpp"


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
    world_ = physicsCommon_.createPhysicsWorld();
}

CollisionDetection::~CollisionDetection() {
    physicsCommon_.destroyPhysicsWorld(world_);
}

void CollisionDetection::AddCollisionBody(entt::entity& entity_id, const glm::vec3& pos, const glm::quat& rot) {
    reactphysics3d::Transform transform = {};
    transform.setPosition(ConvertPosition(pos));
    transform.setOrientation(ConvertRotation(rot));
    reactphysics3d::CollisionBody* body = world_->createCollisionBody(transform);
    AddBodyAndEntt(entity_id, body);
}


void CollisionDetection::AddBodyAndEntt(entt::entity &entity, reactphysics3d::CollisionBody* coll_body) {
    auto r = std::make_pair(entity, coll_body);
    auto y = std::make_pair(coll_body, entity);
    entity_collision_coupling_.insert(r);
    collision_entity_coupling_.insert(y);
}

void CollisionDetection::UpdateCollisionBody(entt::entity &entity_id, const glm::vec3& pos, const glm::quat& rot) {
    auto body = entity_collision_coupling_.at(entity_id);
    reactphysics3d::Transform new_transform(ConvertPosition(pos), ConvertRotation(rot));
    body->setTransform(new_transform);
}



