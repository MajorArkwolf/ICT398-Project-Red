#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <vector>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentID() {
    static ComponentID lastID = 0;
    return lastID++;
}

template<typename T>
inline ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getComponentID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray  = std::array<Component *, maxComponents>;

class Component {
  public:
    Entity *entity = nullptr;

    virtual void init();
    virtual void update();
    virtual void draw();

    virtual ~Component();
};

class Entity {
  public:
    Entity();
    ~Entity();
    void update();
    void draw();
    bool isActive() const;
    void destroy();

    template<typename T>
    bool hasComponent() const {
        return componentBitSet[getComponentTypeID<T>()];
    }
    template<typename T, typename... TArgs>
    T &addComponentID(TArgs &&... mArgs) {
        T *c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()]  = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    template<typename T>
    T &getComponent() const {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T *>(ptr);
    }

  private:
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
};

class Manager {
  private:
    std::vector<std::unique_ptr<Entity>> entities;

  public:
    Manager();
    ~Manager();
    void update();
    void draw();

    void refresh();

    Entity &addEntity();
};

// To add components
// player.addComponent<PositionComponent>();
// player.addComponent<SpriteComponent("assets/player.png");
