#pragma once

#include <entt/entt.hpp>

#include "ECS/ECS.hpp"

class Entity {
   public:
    /**
     * Creates an entity
     * @param entity_id the id for the entity being created.
     * @param ecs_object the entity object system.
     */
    Entity(entt::entity entity_id, ECS *ecs_object);

    /**
     * Add a component to an entity.
     * @tparam T the type
     * @tparam Args the arguments to construct it.
     * @param args the arguments
     * @return reference to the object.
     */
    template<typename T, typename... Args>
    T &AddComponent(Args &&... args) {
        return ecs_->registry_.emplace<T>(this->entity_handle_, std::forward<Args>(args)...);
    }

    /**
     * Returns a reference to a component.
     * @tparam T type of object to return.
     * @return the component of the given type.
     */
    template<typename T>
    T &GetComponent() {
        return ecs_->registry_.get<T>(this->entity_handle_);
    }

    /**
     * Checks if the component exists.
     * @tparam T the component your checking for.
     * @return true or false.
     */
    template<typename T>
    bool HasComponent() {
        return ecs_->registry_.has<T>(this->entity_handle_);
    }

    /**
     * Removes a component.
     * @tparam T the component to remove.
     */
    template<typename T>
    void RemoveComponent() {
        ecs_->registry_.remove<T>(this->entity_handle_);
    }

    entt::entity GetID() const {
        return entity_handle_;
    }

   private:
    /// The id of the given reference.
    entt::entity entity_handle_ = entt::entity{0};
    /// The ecs system that it refers to. Passed in by reference.
    ECS *ecs_ = nullptr;
};
