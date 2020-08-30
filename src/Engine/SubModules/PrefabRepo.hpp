#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>

namespace redengine{
    struct prefab {
        std::string name = "";
        size_t model_id = 0;
        glm::vec3 position_local = {0.0f, 0.0f, 0.0f};
        glm::quat rotation_local = {1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 scale_local = {1.0f, 1.0f, 1.0f};
        //std::vector<>
    };

    class PrefabRepo {
    public:
        PrefabRepo();
        ~PrefabRepo();
        prefab& AddNewPrefab();
    private:
        std::vector<prefab> prefab_list = {};
    };
}

