#include "CollisionDetection.hpp"


static inline glm::vec3 ConvertPosition(const reactphysics3d::Vector3& r_vec) {
    glm::vec3 pos = {};
    pos.x = r_vec.x;
    pos.y = r_vec.y;
    pos.z = r_vec.z;
    return pos;
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

void RedEngineEventListener::onContact(const reactphysics3d::CollisionCallback::CallbackData &callbackData) {
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
            c_p.world_normal = ConvertPosition(contact_point.getWorldNormal());

            // Get the contact point on the first collider and convert it in world-space
            c_p.first_body_contact_point = ConvertPosition(contact_pair.getCollider1()->getLocalToWorldTransform() * contact_point.getLocalPointOnCollider1());
            c_p.second_body_contact_point = ConvertPosition(contact_pair.getCollider1()->getLocalToWorldTransform() * contact_point.getLocalPointOnCollider2());

            p_c_d.contact_points.push_back(c_p);
        }
        physics_que_.push(p_c_d);
    }
}

RedEngineEventListener::RedEngineEventListener(
        std::unordered_map<reactphysics3d::CollisionBody *, entt::entity> *c_e_c) {
    collision_entity_coupling_ = c_e_c;
}

std::queue<PhysicsCollisionData> &RedEngineEventListener::GetPhysicsQueue() {
    return physics_que_;
}

CollisionDetection::CollisionDetection() {
    event_listener_ = RedEngineEventListener(&collision_entity_coupling_);
    world_ = physics_common_.createPhysicsWorld();
    world_->setEventListener(&event_listener_);
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

std::queue<PhysicsCollisionData> &CollisionDetection::GetCollisions() {
    return event_listener_.GetPhysicsQueue();
}

void CollisionDetection::ToggleRenderer() {
    renderer_ = !renderer_;
    world_->setIsDebugRenderingEnabled(renderer_);
    if (renderer_) {
        // Get a reference to the debug renderer
        reactphysics3d::DebugRenderer& debugRenderer = world_->getDebugRenderer();
        // Select the contact points and contact normals to be displayed
        debugRenderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
        debugRenderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    }
}

void CollisionDetection::Draw(const glm::mat4 &projection, const glm::mat4 &view) {
    if (renderer_) {
        reactphysics3d::DebugRenderer& debug_renderer = world_->getDebugRenderer();
        auto line_num = debug_renderer.getNbLines();
        auto triag_num = debug_renderer.getNbTriangles();
        auto line_array = debug_renderer.getLinesArray();
        auto triangle_array = debug_renderer.getTrianglesArray();
    }
}
