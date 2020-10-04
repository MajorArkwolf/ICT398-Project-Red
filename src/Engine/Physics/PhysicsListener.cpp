#include "PhysicsListener.hpp"
#include <glm/gtc/quaternion.hpp>

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

void physics::RedEngineEventListener::onContact(const reactphysics3d::CollisionCallback::CallbackData& callbackData) {
    using namespace reactphysics3d;
    // For each contact pair
    for (uint p = 0; p < callbackData.getNbContactPairs(); p++) {
        PhysicsCollisionData p_c_d = {};
        // Get the contact pair
        CollisionCallback::ContactPair contact_pair = callbackData.getContactPair(p);
        p_c_d.first_body = collision_entity_coupling_->at(contact_pair.getBody1());
        p_c_d.second_body = collision_entity_coupling_->at(contact_pair.getBody2());

        // For each contact point of the contact pair
        for (uint c = 0; c < contact_pair.getNbContactPoints(); c++) {
            ContactPoints c_p = {};
            // Get the contact point
            CollisionCallback::ContactPoint contact_point = contact_pair.getContactPoint(c);
            c_p.penetration = contact_point.getPenetrationDepth();
            c_p.world_normal = ConvertVector(contact_point.getWorldNormal());

            // Get the contact point on the first collider and convert it in world-space
            c_p.first_body_contact_point = ConvertVector(
                    contact_pair.getCollider1()->getLocalToWorldTransform() * contact_point.getLocalPointOnCollider1());
            c_p.second_body_contact_point = ConvertVector(
                    contact_pair.getCollider1()->getLocalToWorldTransform() * contact_point.getLocalPointOnCollider2());

            p_c_d.contact_points.push_back(c_p);
        }

        physics_que_.push(p_c_d);
    }
}

physics::RedEngineEventListener::RedEngineEventListener(
        std::unordered_map<reactphysics3d::CollisionBody*, entt::entity>* c_e_c) {
    collision_entity_coupling_ = c_e_c;
}

std::queue<PhysicsCollisionData>& physics::RedEngineEventListener::GetPhysicsQueue() {
    return physics_que_;
}

float physics::RedEngineRayCast::notifyRaycastHit(const reactphysics3d::RaycastInfo& info) {
    result.is_hit = true;
    result.world_normal = ConvertVector(info.worldNormal);
    result.world_point = ConvertVector(info.worldPoint);
    result.collisionBody = info.body;
    result.collider = info.collider;
    // Display the world hit point coordinates
//    std::cout << "Hit point : " <<
//    info.worldPoint.x <<
//    info.worldPoint.y <<
//    info.worldPoint.z <<
//    std::endl;

    // Return a fraction of 1.0 to gather all hits
    return 1.0f;
}
