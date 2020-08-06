#include "Demo.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Engine.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

Demo::Demo() {
  camera = engine::Camera();
  camera.position_ = glm::vec3(0.0f, 10.0f, 0.0f);
  relativeMouse = true;
  auto entity = JSONLoader::LoadEntity("./res/Entity/Test.json", this->ecs_);
  auto entity2 = JSONLoader::LoadEntity("./res/Entity/Test2.json", this->ecs_);
}

void Demo::Init() {
}

void Demo::UnInit() {
}

void Demo::Display(const glm::mat4& projection, const glm::mat4& view) {
  auto& renderer = redengine::Engine::get().renderer_;
  renderer.SetCameraOnRender(camera);
  ecs_.Draw(projection, view, camera.GetLocation());
}

void Demo::GUIStart() {
}

void Demo::GUIEnd() {
}

void Demo::Update(double t, double dt) {
  ecs_.Update(t, dt);
}

void Demo::FixedUpdate(double t, double dt) {
  ecs_.FixedUpdate(t, dt);
}
