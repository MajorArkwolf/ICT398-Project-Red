#include "CollisionDetection.hpp"

CollisionDetection::CollisionDetection() {
    world_ = physicsCommon_.createPhysicsWorld();
}

void CollisionDetection::AddCollisionBody(entt::entity entity_id) {

}


void CollisionDetection::AddBodyAndEntt(entt::entity &entity, reactphysics3d::CollisionBody& coll_body) {
    entity_to_collision_.insert({entity, coll_body});
    collision_to_entity_.insert({coll_body, entity})
}

