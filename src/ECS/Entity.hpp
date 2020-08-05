#pragma once

#include <entt/entt.hpp>

#include "ECS/ecs.h"

class Entity {
public:
    Entity(entt::entity entityId, ECS* ecsObject);

    template<typename T, typename... Args>
    T &AddComponent(Args &&... args) {
        return ecs->registry.emplace<T>(this->entityHandle, std::forward<Args>(args)...);
    }

    template<typename T>
    T &GetComponent() {
        return ecs->registry.get<T>(this->entityHandle);
    }

    template<typename T>
    bool HasComponent() {
        return ecs->registry.has<T>(this->entityHandle);
    }

    template<typename T>
    void RemoveComponent() {
        ecs->registry.remove<T>(this->entityHandle);
    }

private:
    entt::entity entityHandle = entt::entity{0};
    ECS* ecs = nullptr;
};

