#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <unordered_map>
#include <Engine/Renderer/Shader.hpp>
#include <Engine/Physics/PhysicsShape.hpp>
#include <map>

class PrefabGUI;

namespace redengine{
    struct Collider {
        std::string part_name;
        std::string base_shape_name;
        glm::vec3 position_local = {0.0f, 0.0f, 0.0f};
        glm::quat rotation_local = {1.0f, 0.0f, 0.0f, 0.0f};
        double mass = 0.0;
        glm::vec3 centre_of_mass = {0.0f, 0.0f, 0.0f};
    };

    struct prefab {
        std::string name;
        bool has_model = false;
        size_t model_id = 0;
        std::string model_dir;
        glm::vec3 position_local = {0.0f, 0.0f, 0.0f};
        glm::quat rotation_local = {1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 scale_local = {1.0f, 1.0f, 1.0f};
        bool has_animation = false;
        bool has_physics = false;
        bool is_static = false;
        std::map<std::string, physics::PhysicsShape> collision_shapes = {};
        std::vector<Collider> colliders_;
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
        const prefab &GetPrefab(const std::string &key) const;
        std::vector<std::string> GetPrefabList ();

    private:
        std::unordered_map<std::string,prefab> prefabMap_ = {};
        void InsertPrefab(const prefab& new_prefab);
        prefab &GetPrefabMut(const std::string &key);
        //Friend class access to be able to modify Repo
        friend PrefabGUI;
    };
}

