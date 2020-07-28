#include "Entity.hpp"


Entity::Entity(entt::entity entityId, ECS *ecsObject) {
    this->entityHandle = entityId;
    this->ecs = ecsObject;
}

