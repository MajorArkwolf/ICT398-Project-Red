#include "Ecs.hpp"
#include "ECS/System/Draw.hpp"
#include "ECS/Entity.hpp"

void ECS::FixedUpdate(double t, double dt) {

}

void ECS::Update(double t, double dt) {

}

void ECS::Draw(const glm::mat4& projection, const glm::mat4& view, const glm::dvec2& cameraPos) {
    System::Draw(registry, projection, view);
}

Entity &ECS::CreateEntity() {
    this->entityRegister.emplace_back(std::make_shared<Entity>(registry.create(), this));
    return *entityRegister.at(entityRegister.size() - 1).get();
}

