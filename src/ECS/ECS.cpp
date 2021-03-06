#include "ECS.hpp"

#include "ECS/Entity.hpp"
#include "ECS/System/System.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "ECS/System/AISystem.hpp"

void ECS::FixedUpdate(double t, double dt) {
    System::AISystem::Moving(registry_, t, dt);
}

void ECS::Update(double t, double dt) {
    System::UpdateAnimation(registry_, t, dt);
    System::UpdateColors(registry_);
}

void ECS::Draw(Shader *shader, const glm::mat4& projection, const glm::mat4& view) {
  System::Draw(registry_, shader, projection, view);
}

std::shared_ptr<Entity> ECS::GetEntity(entt::entity id) {
    for (auto& n : entity_register_) {
        if (n->GetID() == id) {
            return n;
        }
    }
    return nullptr;
}

Entity& ECS::CreateEntity() {
    this->entity_register_.emplace_back(
        std::make_shared<Entity>(registry_.create(), this));
    return *entity_register_.at(entity_register_.size() - 1).get();
}
