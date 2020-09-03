#include "CollisionDetection.hpp"
#include "Engine/Engine.hpp"

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
    using reactphysics3d::DebugRenderer;
    event_listener_ = RedEngineEventListener(&collision_entity_coupling_);
    world_ = physics_common_.createPhysicsWorld();
    world_->setEventListener(&event_listener_);
    auto base_path = redengine::Engine::get().GetBasePath();
    auto vs = base_path / "res" / "shader" / "react_shader.vs";
    auto fs = base_path / "res" / "shader" / "react_shader.fs";
    shader_ = std::make_unique<Shader>(vs, fs, "");

    //Generate line buffers for test renderer
    glGenVertexArrays(1, &l_vao_);
    glGenBuffers(1, &l_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Generate triangle buffers for test renderer
    glGenVertexArrays(1, &t_vao_);
    glGenBuffers(1, &t_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
    // Get a reference to the debug renderer
    reactphysics3d::DebugRenderer& debug_renderer = world_->getDebugRenderer();
    if (renderer_) {
        // Select the contact points and contact normals to be displayed
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    } else {
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, false);
        debug_renderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, false);
    }
}

void CollisionDetection::Draw(const glm::mat4 &projection, const glm::mat4 &view) {
    if (renderer_) {
        //TODO Setup the shader, verify data is okay being passed in like this.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        shader_->Use();
        shader_->SetMat4("view", view);
        shader_->SetMat4("projection", projection);
        auto model = glm::transpose(glm::inverse(view));
        shader_->SetMat4("model", model);

        // Lines
        if (line_num_ > 0) {
            // Bind the VAO
            glBindVertexArray(l_vao_);
            glBindVertexArray(l_vbo_);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (char*)nullptr);

            glEnableVertexAttribArray(1);
            glVertexAttribIPointer(1, 3, GL_UNSIGNED_INT, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (void*)sizeof(rp3d::Vector3));

            // Draw the lines geometry
            glDrawArrays(GL_LINES, 0, line_num_ * 2);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        if (triag_num_ > 0) {

            // Bind the VAO
            glBindVertexArray(t_vao_);
            glBindVertexArray(t_vbo_);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (char*)nullptr);

            glEnableVertexAttribArray(1);
            glVertexAttribIPointer(1, 3, GL_UNSIGNED_INT, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (void*)sizeof(rp3d::Vector3));

            // Draw the triangles geometry
            glDrawArrays(GL_TRIANGLES, 0, triag_num_ * 3);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void CollisionDetection::Update(double t, double dt) {
    if (renderer_) {
        reactphysics3d::DebugRenderer& debug_renderer = world_->getDebugRenderer();
        if (debug_renderer.getNbLines() > 0) {
            line_num_ = debug_renderer.getNbLines();
            glBufferData(GL_ARRAY_BUFFER, line_num_ * sizeof(reactphysics3d::DebugRenderer::DebugLine), debug_renderer.getLinesArray(), GL_STREAM_DRAW);
        }
        if (debug_renderer.getNbTriangles() > 0) {
            triag_num_ = debug_renderer.getNbTriangles();
            glBufferData(GL_ARRAY_BUFFER, triag_num_ * sizeof(reactphysics3d::DebugRenderer::DebugTriangle), debug_renderer.getTrianglesArray(), GL_STREAM_DRAW);
        }
    }
}


bool CollisionDetection::GetRendererStatus() const {
    return renderer_;
}

PhysicsShape CollisionDetection::CreateBoxShape(glm::vec3 extents) {
    return PhysicsShape(physics_common_.createBoxShape(ConvertPosition(extents)));
}

PhysicsShape CollisionDetection::CreateCapsuleShape(double radius, double height) {
    return PhysicsShape(physics_common_.createCapsuleShape(radius, height));
}

PhysicsShape CollisionDetection::CreateSphereShape(double radius) {
    return PhysicsShape(physics_common_.createSphereShape(radius));
}

int CollisionDetection::AddCollider(const entt::entity& entity_id, PhysicsShape& shape, glm::vec3 relative_position, glm::quat rotation) {
    auto* body = entity_collision_coupling_.at(entity_id);
    rp3d::Transform transform(ConvertPosition(relative_position), ConvertRotation(rotation));
    body->addCollider(shape.shape_, transform);
    return body->getNbColliders();

}
