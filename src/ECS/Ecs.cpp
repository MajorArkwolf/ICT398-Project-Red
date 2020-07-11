#include "Ecs.hpp"
#include "ECS/System/Draw.hpp"

void ECS::FixedUpdate(double t, double dt) {

}

void ECS::Update(double t, double dt) {

}

void ECS::Draw(const glm::mat4& projection, const glm::mat4& view, const glm::dvec2& cameraPos) {
    System::Draw(registry, projection, view);
}
