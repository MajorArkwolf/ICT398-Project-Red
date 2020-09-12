#include "ECS.hpp"

#include "ECS/Entity.hpp"
#include "ECS/System/System.hpp"
#include "Engine/Renderer/Shader.hpp"

void ECS::FixedUpdate(double t, double dt) {
}

void ECS::Update(double t, double dt) {
  System::UpdateAnimation(registry_, t, dt);
}

void ECS::Draw(Shader *shader, const glm::mat4& projection, const glm::mat4& view) {
  System::Draw(registry_, shader, projection, view);
}

Entity& ECS::CreateEntity() {
  this->entity_register_.emplace_back(
      std::make_shared<Entity>(registry_.create(), this));
  return *entity_register_.at(entity_register_.size() - 1).get();
}
