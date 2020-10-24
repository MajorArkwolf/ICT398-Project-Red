#include "PhysicsEngine.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Player.hpp"
#include "ECS/ECS.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Physics/CollisionDetection.hpp"

using namespace physics;

static inline glm::vec3 ConvertVector(const reactphysics3d::Vector3 &r_vec) {
    glm::vec3 pos = {};
    pos.x = r_vec.x;
    pos.y = r_vec.y;
    pos.z = r_vec.z;
    return pos;
}

static inline reactphysics3d::Vector3 ConvertVector(const glm::vec3 &glm_vec) {
    auto pos = reactphysics3d::Vector3();
    pos.x = glm_vec.x;
    pos.y = glm_vec.y;
    pos.z = glm_vec.z;
    return pos;
}

static inline reactphysics3d::Quaternion ConvertQuaternion(const glm::quat &glm_quat) {
    auto rot = reactphysics3d::Quaternion();
    rot.w = glm_quat.w;
    rot.x = glm_quat.x;
    rot.y = glm_quat.y;
    rot.z = glm_quat.z;
    return rot;
}

static inline glm::quat ConvertQuaternion(const reactphysics3d::Quaternion &react_quat) {
    auto rot = glm::quat();
    rot.w = react_quat.w;
    rot.x = react_quat.x;
    rot.y = react_quat.y;
    rot.z = react_quat.z;
    return rot;
}

void PhysicsEngine::FixedUpdate(double t, double dt) {
    collision_detection_.FixedUpdate(t, dt);
    IntegrateVelocities(dt);
    IntegratePositions(dt);
    collision_resolution_.Resolve(collision_detection_.GetCollisions(), t, dt);

    ResetAddedForces();
}

void PhysicsEngine::Update(double t, double dt) {
    collision_detection_.Update(t, dt);
}

void PhysicsEngine::Draw(Shader *shader, const glm::mat4 &projection, const glm::mat4 &view) {
    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    auto &ecs = physics_world.ecs_;
    if (ecs != nullptr) {
        collision_detection_.Draw(shader, projection, view);
    }
}

PhysicsShape PhysicsEngine::CreateBoxShape(glm::vec3 extents) {
    return collision_detection_.CreateBoxShape(extents);
}

PhysicsShape PhysicsEngine::CreateCapsuleShape(float radius, float height) {
    return collision_detection_.CreateCapsuleShape(radius, height);
}

PhysicsShape PhysicsEngine::CreateSphereShape(double radius) {
    return collision_detection_.CreateSphereShape(radius);
}

bool PhysicsEngine::GetRendererStatus() {
    return collision_detection_.GetRendererStatus();
}

reactphysics3d::PhysicsWorld *PhysicsEngine::CreatePhysicsWorld() {
    return collision_detection_.CreatePhysicsWorld();
}

void PhysicsEngine::DestroyPhysicsWorld(reactphysics3d::PhysicsWorld *world) {
    collision_detection_.DeletePhysicsWorld(world);
}

glm::mat3x3 physics::PhysicsEngine::CalculateInertiaTensor(redengine::Box shape, float mass) {
    glm::mat3x3 tensor{};
    constexpr float calc = 1.f / 12.f;
    //length
    tensor[0][0] = calc * mass * (shape.extents.y * shape.extents.y + shape.extents.x * shape.extents.x);
    //height
    tensor[1][1] = calc * mass * (shape.extents.z * shape.extents.z + shape.extents.x * shape.extents.x);
    //width
    tensor[2][2] = calc * mass * (shape.extents.z * shape.extents.z + shape.extents.y * shape.extents.y);
    return tensor;
}

glm::mat3x3 physics::PhysicsEngine::CalculateInertiaTensor(redengine::Capsule shape, float mass) {

    glm::mat3x3 tensor{};
    constexpr float calc = 2.f / 5.f;
    auto calculation = calc * mass * float(shape.radius * shape.radius);
    tensor[0][0] = calculation;
    tensor[1][1] = calculation;
    tensor[2][2] = calculation;
    return tensor;
}

glm::mat3x3 physics::PhysicsEngine::CalculateInertiaTensor(redengine::Sphere shape, float mass) {
    glm::mat3x3 tensor{};
    constexpr float calc = 2.f / 5.f;
    auto calculation = calc * mass * float(shape.radius * shape.radius);
    tensor[0][0] = calculation;
    tensor[1][1] = calculation;
    tensor[2][2] = calculation;
    return tensor;
}

void PhysicsEngine::Init() {
    collision_detection_.Init();
    //collision_resolution_.Init();
}

entt::entity PhysicsEngine::RayCastSingle(const glm::vec3 &start, const glm::vec3 &end) {
    return collision_detection_.RayCastSingle(start, end);
}

entt::entity PhysicsEngine::RayCastSingle(const glm::vec3 &start, const glm::vec3 &front, float distance) {
    glm::vec3 end = start;
    end += distance * front;
    return collision_detection_.RayCastSingle(start, end);
}

void PhysicsEngine::SetTrigger(entt::entity entity, bool is_trigger) {
    collision_detection_.SetTrigger(entity, is_trigger);
}

void physics::PhysicsEngine::IntegrateVelocities(double dt) {
    constexpr float angular_damping = 0.1f;
    const float damping_factor = std::pow(1.0 - angular_damping, static_cast<float>(dt));
    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    auto &ecs = physics_world.ecs_;
    if (ecs != nullptr) {
        auto &registry = ecs->GetRegistry();
        auto entities = registry.view<component::Transform, component::PhysicBody>();

        for (auto &e : entities) {
            auto &phys_body = entities.get<component::PhysicBody>(e);

            auto &linear_velocity = phys_body.linear_velocity;
            auto &angular_velocity = phys_body.angular_velocity;

            linear_velocity += static_cast<float>(dt) * (phys_body.inverse_mass * phys_body.added_force);
            angular_velocity += static_cast<float>(dt) * (phys_body.inverse_inertia_tensor * phys_body.added_torque);
           // angular_velocity *= damping_factor;
        }

        if (physics_world.IsGravityEnabled()) {
            for (auto &e : entities) {
                auto &phys_body = entities.get<component::PhysicBody>(e);
                if (!phys_body.static_object && !phys_body.is_sleeping && phys_body.should_apply_gravity) {
                    auto &linear_velocity = phys_body.linear_velocity;

                    linear_velocity += static_cast<float>(dt) * (phys_body.inverse_mass * phys_body.mass * physics_world.GetGravity());
                }
            }
        }
    }
}

void physics::PhysicsEngine::IntegratePositions(double dt) {
    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    auto &ecs = physics_world.ecs_;
    if (ecs != nullptr) {
        auto &registry = ecs->GetRegistry();
        auto entities = registry.view<component::Transform, component::PhysicBody>();

        for (auto &e : entities) {
            auto &tran = entities.get<component::Transform>(e);
            auto &phys_body = entities.get<component::PhysicBody>(e);

            auto &linear_velocity = phys_body.linear_velocity;
            auto &angular_velocity = phys_body.angular_velocity;

            if (!phys_body.static_object) {
                tran.pos += linear_velocity * static_cast<float>(dt);
                //tran.rot = glm::quat(angular_velocity) * tran.rot * static_cast<float>(dt);
                //tran.rot += glm::quat(1.0, angular_velocity) * tran.rot * 0.5f * static_cast<float>(dt);
                tran.rot = tran.rot + glm::quat(0.0, angular_velocity) * tran.rot * 0.5f * static_cast<float>(dt);
                //tran.rot += (tran.rot * 0.5f * glm::quat(1.0, angular_velocity) * static_cast<float>(dt));
                tran.rot = glm::normalize(tran.rot);
            }
            physics_world.UpdateCollisionBody(e, tran.pos, tran.rot);
        }
    }
}

void physics::PhysicsEngine::ResetAddedForces() {
    auto &physics_world = redengine::Engine::get().game_stack_.getTop()->physics_world_;
    auto &ecs = physics_world.ecs_;
    if (ecs != nullptr) {
        auto &registry = ecs->GetRegistry();
        auto entities = registry.view<component::Transform, component::PhysicBody>();

        for (auto &e : entities) {
            auto &phys_body = entities.get<component::PhysicBody>(e);

            phys_body.added_force = {0.f, 0.f, 0.f};
            phys_body.added_torque = {0.f, 0.f, 0.f};
            phys_body.should_apply_gravity = true;
        }
    }
}
