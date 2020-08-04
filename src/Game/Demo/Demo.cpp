#include "ECS/Entity.hpp"
#include "Demo.hpp"
#include "Engine/Engine.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Basic.hpp"
#include "Engine/SubModules/JsonLoader.hpp"


// TEST
#include "Engine/Engine.hpp"

Demo::Demo() {
    camera = Engine::Camera();
    camera.Position = glm::vec3(0.0f, 10.0f, 0.0f);
    relativeMouse = true;
    auto entity = JSONLoader::LoadEntity("./res/Entity/Test.json", this->ecs);
    auto entity2 = JSONLoader::LoadEntity("./res/Entity/Test2.json", this->ecs);
}

void Demo::Init() {

}

void Demo::UnInit() {

}

void Demo::Display(const glm::mat4& projection, const glm::mat4& view) {
    auto &renderer = redengine::Engine::get().renderer_;
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
