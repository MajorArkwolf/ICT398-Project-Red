#include "ECS.hpp"

/**
 * @brief  virtual init function for components
 */
void Component::init() {}

/**
 * @brief  virtual update function for components
 */
void Component::update() {}

/**
 * @brief  virtual draw function for components
 */
void Component::draw() {}

/**
 * @brief  Destructor for Component
 */
Component::~Component() {}

/**
 * @brief  Default Constructor for Entity
 */
Entity::Entity()  = default;

/**
 * @brief  Default Constructor for Entity
 */
Entity::~Entity() = default;

/**
 * @brief  Iterates through all the components update function
 */
void Entity::update() {
    for (auto &c : components)
        c->update();
}

/**
 * @brief  Iterates through all the components draw function
 */
void Entity::draw() {
    for (auto &c : components)
        c->draw();
}

/**
 * @brief  Checks to see if the entity is still active
 * @return if the entity is still active
 */
bool Entity::isActive() const {
    return active;
}

/**
 * @brief  Destroys the entity
 */
void Entity::destroy() {
    active = false;
}

/**
 * @brief  Default Constructor for Manager
 */
Manager::Manager()  = default;

/**
 * @brief  Default Constructor for Manager
 */
Manager::~Manager() = default;

/**
 * @brief  Iterates through all the entities update functions
 */
void Manager::update() {
    for (auto &e : entities)
        e->update();
}

/**
 * @brief  Iterates through all the entities draw functions
 */
void Manager::draw() {
    for (auto &e : entities)
        e->draw();
}

/**
 * @brief  Iterates through all the entities to see if they are still active, otherwise destroys them.
 */
void Manager::refresh() {
    entities.erase(std::remove_if(std::begin(entities), std::end(entities),
                                  [](const std::unique_ptr<Entity> &mEntity) {
                                      return !mEntity->isActive();
                                  }),
                   std::end(entities));
}

/**
 * @brief  Add an entity to the manager
 * @return a pointer to the entity for external use
 */
Entity &Manager::addEntity() {
    Entity *e = new Entity();
    std::unique_ptr<Entity> uPtr{e};
    entities.emplace_back(std::move(uPtr));
    return *e;
}
