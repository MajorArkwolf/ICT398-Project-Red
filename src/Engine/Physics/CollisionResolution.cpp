#include "CollisionResolution.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Player.hpp"
#include "ECS/ECS.hpp"

void physics::CollisionResolution::Resolve(std::queue<PhysicsCollisionData>& queue, double t, double dt) {

    auto HasComponent = [&](entt::entity id, auto arg) {
        return ecs_->GetRegistry().has<arg>(id);
    };

    while (!queue.empty()) {
        auto& item = queue.front();
        if (ecs_) {
            if (ecs_->GetRegistry().has<component::Player>(item.first_body)) {
                auto& ref = ecs_->GetRegistry().get<component::Player>(item.first_body);
            }

        }
    }
}

void physics::CollisionResolution::SetECS(ECS* ecs) {
    this->ecs_ = ecs;
}
