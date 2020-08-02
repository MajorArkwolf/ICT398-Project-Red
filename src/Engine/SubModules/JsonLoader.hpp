#pragma once
#include <memory>
#include <optional>
#include <nlohmann/json.hpp>
#include <filesystem>

class Entity;
class ECS;

class JSONLoader {
public:
    static nlohmann::json LoadJson(const std::filesystem::path& filepath);
    static std::optional<std::shared_ptr<Entity>> LoadEntity(const std::filesystem::path& filepath, ECS& ecs);
    static void LoadScene(const std::filesystem::path& filepath, ECS& ecs);
};

