#include "Demo.hpp"
#include "Engine/Engine.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Basic.hpp"

Demo::Demo() {
    camera = Engine::Camera();
    camera.Position = glm::vec3(0.0f, 10.0f, 0.0f);
    relativeMouse = true;
    auto entity = ecs.registry.create();
    auto shader = std::make_shared<Shader>(Shader("res/shader/vertshader.vs", "res/shader/fragshader.fs"));
    ecs.registry.emplace<Component::Model>(entity, "res/model/ClothedMan.gltf", shader);
    auto &tran = ecs.registry.emplace<Component::Transform>(entity);
    tran.pos.x = 20.0f;
    tran.pos.z = 2.0f;
}

void Demo::Init() {

}

void Demo::UnInit() {

}

void Demo::Display(glm::mat4 projection, glm::mat4 view) {
    auto &engine = RedEngine::Engine::get();
    auto &renderer = RedEngine::Engine::get().renderer;
    renderer.SetCameraOnRender(camera);
    ecs.Draw(projection, view, camera.getLocation());
}

void Demo::GUIStart() {

}

void Demo::GUIEnd() {

}

void Demo::Update(double t, double dt) {
    ecs.Update(t, dt);
}

void Demo::FixedUpdate(double t, double dt) {
    ecs.FixedUpdate(t, dt);
}
