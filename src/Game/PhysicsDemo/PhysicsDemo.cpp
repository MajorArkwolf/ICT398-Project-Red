#include "PhysicsDemo.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Engine.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

PhysicsDemo::PhysicsDemo() {
    physics_engine_.SetECS(&ecs_);
    camera = engine::Camera();
    camera.position_ = glm::vec3(0.0f, 10.0f, 0.0f);
    relativeMouse = true;
    auto entity = JSONLoader::LoadEntity("./res/Entity/Test.json", this->ecs_);
    auto entity2 = JSONLoader::LoadEntity("./res/Entity/Test2.json", this->ecs_);
}

void PhysicsDemo::Display(const glm::mat4& projection, const glm::mat4& view) {
    auto& renderer = redengine::Engine::get().renderer_;
    renderer.SetCameraOnRender(camera);
    ecs_.Draw(projection, view, camera.GetLocation());
}

void PhysicsDemo::GUIStart() {

}

void PhysicsDemo::GUIEnd() {
}

void PhysicsDemo::Update(double t, double dt) {
    ecs_.Update(t, dt);
    physics_engine_.Update(t, dt);
}

void PhysicsDemo::FixedUpdate(double t, double dt) {
    ecs_.FixedUpdate(t, dt);
    physics_engine_.FixedUpdate(t, dt);
}

void PhysicsDemo::Init() {

}

void PhysicsDemo::UnInit() {

}
