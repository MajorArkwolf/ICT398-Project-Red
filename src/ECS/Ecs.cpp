#include "Ecs.hpp"
#include "ECS/System/Draw.hpp"

void ECS::FixedUpdate(double t, double dt) {

}

void ECS::Update(double t, double dt) {

}

void ECS::Draw() {
    System::Draw(registry);
}
