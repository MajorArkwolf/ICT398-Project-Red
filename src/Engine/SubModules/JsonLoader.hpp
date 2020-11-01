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
    enum class JsonType {
        String,
        Boolean,
        Float,
        Int,
        Array,
        Json,
        Number
    };

   public:
    /**
    * Loads a json file
    * @param file_path The path of the file to load
    */
    static nlohmann::json LoadJson(const std::filesystem::path& file_path);
    static std::optional<std::shared_ptr<Entity>> LoadEntity(
        const std::filesystem::path& file_path, ECS* ecs, physics::PhysicsWorld* pw = nullptr);        // NOLINT
    static void LoadScene(const std::filesystem::path& file_path, ECS* ecs, physics::PhysicsWorld* pw);// NOLINT
    static void LoadPrefabList();

    private:
    /**
    * Gets the json data from a json field, creating the appropriate log errors if not found
    * @param input_json The json object to read from
    * @param json_name The name of the json object to throw error logs from
    * @param field_name The name of the field to look for in the passed in json object
    * @param expectedType The expected type of field to look for.
    */
    static std::optional<std::reference_wrapper<nlohmann::json>> GetJsonField(nlohmann::json& input_json, const std::string &json_name, const std::string &field_name, JsonType expectedType);
};
