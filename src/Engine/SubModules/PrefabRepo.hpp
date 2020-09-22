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
        /**
         * Default Constructor.
         */
        PrefabRepo();

        /**
         * Default Destructor.
         */
        ~PrefabRepo() = default;

        /**
         * Deleted copy constructor
         * @param prefabRepo
         */
        PrefabRepo(PrefabRepo& prefabRepo) = delete;

        /**
         * Deleted Move Constructor
         * @param prefabRepo
         */
        PrefabRepo(PrefabRepo&& prefabRepo) = default;

        /**
         * Deleted Copy Assignment
         * @param prefabRepo
         * @return
         */
        PrefabRepo& operator=(PrefabRepo& prefabRepo) = delete;

        /**
         * Move assignment
         * @param prefabRepo
         * @return
         */
        PrefabRepo& operator=(PrefabRepo&& prefabRepo) = default;

        /**
         * Init function to construction things that cant happen on instantiation.
         */
        void Init();

        /**
         * Add a new prefab
         * @param key Give it the key
         * @return returns an empty prefab to be filled.
         */
        prefab& AddNewPrefab(const std::string& key);

        /**
         * Check to see if a prefab exists.
         * @param key to search for
         * @return true if it exists in the array
         */
        bool FindPrefab(const std::string& key);

        /**
         * Get Prefab of key
         * @param key to search for
         * @return returns a const prefab.
         */
        const prefab &GetPrefab(const std::string &key) const;

        std::vector<std::string> GetPrefabList ();

    private:
        /// Map of prefabs
        std::unordered_map<std::string,prefab> prefabMap_ = {};

        /**
         * Inserts a prefab, uses the name stored inside the prefab as the key.
         * @param new_prefab prefab to be stored.
         */
        void InsertPrefab(const prefab& new_prefab);

        /**
         * Get Prefab
         * @param key to the prefab
         * @return a mutable prefab by reference.
         */
        prefab &GetPrefabMut(const std::string &key);
        //Friend class access to be able to modify Repo
        friend PrefabGUI;
    };
}

