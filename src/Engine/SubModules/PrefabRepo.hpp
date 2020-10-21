#pragma once

#include <Engine/Physics/PhysicsShape.hpp>
#include <Engine/Renderer/Shader.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <map>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <nlohmann/json.hpp>

namespace redengine {
struct Sphere {
    double radius = 0;
};
struct Box {
    glm::vec3 extents = {};
};
struct Capsule {
    float radius = 0;
    float height = 0;
};
struct Collider {
    std::variant<std::monostate, Sphere, Box, Capsule> shape;
    std::string part_name;
    std::string base_shape_name;
    glm::vec3 position_local = {0.0f, 0.0f, 0.0f};
    glm::quat rotation_local = {1.0f, 0.0f, 0.0f, 0.0f};
    float mass = 0.0;
    glm::vec3 centre_of_mass = {0.0f, 0.0f, 0.0f};
};

struct prefab {
    bool render = true;
    std::string name;
    std::string file_name;
    std::string model_dir;
    bool has_model = false;
    size_t model_id = 0;
    std::shared_ptr<Shader> model_shader;
    glm::vec3 position_local = {0.0f, 0.0f, 0.0f};
    glm::quat rotation_local = {1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale_local = {1.0f, 1.0f, 1.0f};
    bool has_animation = false;
    bool has_physics = false;
    bool is_static = false;
    std::map<std::string, physics::PhysicsShape> collision_shapes = {};
    std::vector<Collider> colliders_;
    float mass = 0.0;
    glm::dvec3 centre_of_mass = {0, 0, 0};
    static void to_json(nlohmann::json& json, const prefab& fab);
    std::vector<std::string> GetColliderList();

};

class PrefabRepo {
   public:
    PrefabRepo();
    ~PrefabRepo() = default;
    PrefabRepo(PrefabRepo& prefabRepo) = delete;
    PrefabRepo(PrefabRepo&& prefabRepo) = default;
    PrefabRepo& operator=(PrefabRepo& prefabRepo) = delete;
    PrefabRepo& operator=(PrefabRepo&& prefabRepo) = default;
    void Init();
    prefab& AddNewPrefab(const std::string& key);
    bool FindPrefab(const std::string& key);
    const prefab& GetPrefab(const std::string& key) const;
    prefab &GetPrefabMut(const std::string &key);
    std::vector<std::string> GetPrefabList();
    void InsertPrefab(const prefab &new_prefab);

   private:
    std::unordered_map<std::string, prefab> prefabMap_ = {};


};
}// namespace redengine
