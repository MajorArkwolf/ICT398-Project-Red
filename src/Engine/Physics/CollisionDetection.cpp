#include "CollisionDetection.hpp"

#include "Engine/Engine.hpp"
#include "Engine/Physics/PhysicsCommon.hpp"

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

CollisionDetection::CollisionDetection() {
    //physics_common_->setLogger(&logger_);

}

void CollisionDetection::Init() {
    using reactphysics3d::DebugRenderer;
    auto base_path = redengine::Engine::get().GetBasePath();
    auto vs = base_path / "res" / "shader" / "react_shader.vs";
    auto fs = base_path / "res" / "shader" / "react_shader.fs";
    shader_ = std::make_unique<Shader>(vs, fs, "");

    //Generate line buffers for test renderer
    glGenVertexArrays(1, &l_vao_);
    assert(l_vao_ != 0);
    glGenBuffers(1, &l_vbo_);
    assert(l_vbo_ != 0);


    //Generate triangle buffers for test renderer
    glGenVertexArrays(1, &t_vao_);
    assert(t_vao_ != 0);
    glGenBuffers(1, &t_vbo_);
    assert(t_vbo_ != 0);
};

CollisionDetection::~CollisionDetection() {
}

std::queue<PhysicsCollisionData>& CollisionDetection::GetCollisions() {
    return redengine::Engine::get().game_stack_.getTop()->physics_world_.event_listener_.GetPhysicsQueue();
}

void CollisionDetection::Draw(Shader *shader, const glm::mat4& projection, const glm::mat4& view) {
    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    if (physics_world.renderer_) {
        shader_->Use();
        //TODO Setup the shader, verify data is okay being passed in like this.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        shader_->SetMat4("projection", projection);
        shader_->SetMat4("view", view);
        shader_->SetMat4("model", view);
//        const glm::mat4 localToCameraMatrix = view;
//        const glm::mat4 normalMatrix = glm::transpose(glm::inverse(localToCameraMatrix));
//        shader_->SetMat4("normalMatrix", normalMatrix);
//
//        // Set the model to camera matrix
//        shader_->SetMat4("localToWorldMatrix", glm::mat4());
//        shader_->SetMat4("worldToCameraMatrix", view);
//        shader_->SetBool("isGlobalVertexColorEnabled", false);



        // Lines
        if (line_num_ > 0) {
            // Bind the VAO
            glBindVertexArray(l_vao_);
            glBindBuffer(GL_ARRAY_BUFFER, l_vbo_);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (char*) nullptr);

            glEnableVertexAttribArray(1);
            glVertexAttribIPointer(1, 3, GL_UNSIGNED_INT, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (void*) sizeof(rp3d::Vector3));

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
            glBindBuffer(GL_ARRAY_BUFFER, t_vbo_);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (char*) nullptr);

            glEnableVertexAttribArray(1);
            glVertexAttribIPointer(1, 3, GL_UNSIGNED_INT, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (void*) sizeof(rp3d::Vector3));

            // Draw the triangles geometry
            glDrawArrays(GL_TRIANGLES, 0, triag_num_ * 3);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader->Use();
    }
}

void CollisionDetection::FixedUpdate(double t, double dt) {
    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    physics_world.world_->update(dt);
}

void CollisionDetection::Update(double t, double dt) {
    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    //physics_world.world_->update(dt);
    if (physics_world.renderer_) {
        reactphysics3d::DebugRenderer& debug_renderer = physics_world.world_->getDebugRenderer();
        line_num_ = debug_renderer.getNbLines();
        if (line_num_ > 0) {
            glBindBuffer(GL_ARRAY_BUFFER, l_vbo_);
            auto sizeVertices = static_cast<GLsizei>(line_num_ * sizeof(rp3d::DebugRenderer::DebugLine));
            glBufferData(GL_ARRAY_BUFFER, sizeVertices, debug_renderer.getLinesArray(), GL_STREAM_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        triag_num_ = debug_renderer.getNbTriangles();
        if (triag_num_ > 0) {
            glBindBuffer(GL_ARRAY_BUFFER, t_vbo_);
            auto sizeVertices = static_cast<GLsizei>(triag_num_ * sizeof(rp3d::DebugRenderer::DebugTriangle));
            glBufferData(GL_ARRAY_BUFFER, sizeVertices, debug_renderer.getTrianglesArray(), GL_STREAM_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}

bool CollisionDetection::GetRendererStatus() const {
    return redengine::Engine::get().game_stack_.getTop()->physics_world_.renderer_;
}

PhysicsShape CollisionDetection::CreateBoxShape(glm::vec3 extents) {
    return PhysicsShape(physics_common_.createBoxShape(ConvertVector(extents)), ShapeType::Box);
}

PhysicsShape CollisionDetection::CreateCapsuleShape(double radius, double height) {
    return PhysicsShape(physics_common_.createCapsuleShape(radius, height), ShapeType::Capsule);
}

PhysicsShape CollisionDetection::CreateSphereShape(double radius) {
    return PhysicsShape(physics_common_.createSphereShape(radius), ShapeType::Sphere);
}

reactphysics3d::PhysicsWorld *CollisionDetection::CreatePhysicsWorld() {
    return physics_common_.createPhysicsWorld();
}

void CollisionDetection::DeletePhysicsWorld(reactphysics3d::PhysicsWorld *world) {
    physics_common_.destroyPhysicsWorld(world);
}
