#include "CollisionResolution.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Player.hpp"
#include "ECS/ECS.hpp"

void physics::CollisionResolution::Resolve(std::queue<PhysicsCollisionData>& queue, double t, double dt) {

    while (!queue.empty()) {
        auto& item = queue.front();
        int player = 0;
        if (ecs_) {
            if (ecs_->GetRegistry().has<component::Player>(item.first_body)) {
                player = 1;
            } else if (ecs_->GetRegistry().has<component::Player>(item.second_body)) {
                player = 2;
            }
        }

        queue.pop();
    }
}

void physics::CollisionResolution::SetECS(ECS* ecs) {
    this->ecs_ = ecs;
}
