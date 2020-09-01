#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <unordered_map>

namespace redengine{
    struct prefab {
        std::string name;
        size_t model_id = 0;
        glm::vec3 position_local = {0.0f, 0.0f, 0.0f};
        glm::quat rotation_local = {1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 scale_local = {1.0f, 1.0f, 1.0f};
        //std::vector<>
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
    private:
        std::unordered_map<std::string,prefab> prefabMap_ = {};
    };
}

