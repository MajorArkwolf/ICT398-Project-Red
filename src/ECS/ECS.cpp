#include "ECS.hpp"

#include "ECS/Entity.hpp"
#include "ECS/System/System.hpp"

void ECS::FixedUpdate(double t, double dt) {
}

void ECS::Update(double t, double dt) {
  System::UpdateAnimation(registry, t, dt);
}

void ECS::Draw(const glm::mat4& projection, const glm::mat4& view, const glm::dvec2& cameraPos) {
  System::Draw(registry, projection, view);
}

Entity& ECS::CreateEntity() {
  this->entity_register.emplace_back(std::make_shared<Entity>(registry.create(), this));
  return *entity_register.at(entity_register.size() - 1).get();
}
