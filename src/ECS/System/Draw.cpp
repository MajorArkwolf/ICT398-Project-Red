#include "Draw.hpp"
#include "ECS/Component/Model.hpp"

void System::Draw(const entt::registry& registry) {
    auto model = registry.view<Component::Model>();

    for (auto& entity: model) {

    }
}
