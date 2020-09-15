#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include <nlohmann/json.hpp>

class Entity;
class ECS;
namespace physics {
    class PhysicsEngine;
    class PhysicsWorld;
}

class JSONLoader {
   public:
    static nlohmann::json LoadJson(const std::filesystem::path& file_path);
    static std::optional<std::shared_ptr<Entity>> LoadEntity(
        const std::filesystem::path& file_path, ECS* ecs, physics::PhysicsWorld* pw = nullptr);        // NOLINT
    static void LoadScene(const std::filesystem::path& file_path, ECS* ecs, physics::PhysicsWorld* pw);// NOLINT
    static void LoadPrefabList();
};
