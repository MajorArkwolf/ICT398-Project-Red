#pragma once
#include <memory>
#include <optional>

class Entity;
class ECS;

class JSONLoader {
public:
    static std::optional<std::shared_ptr<Entity>> LoadEntity(const std::string& filepath, ECS& ecs);
};

