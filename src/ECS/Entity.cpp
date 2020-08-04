#include "Entity.hpp"


Entity::Entity(entt::entity entity_id, ECS *ecs_object) {
    this->entity_handle_ = entity_id;
    this->ecs_ = ecs_object;
}

