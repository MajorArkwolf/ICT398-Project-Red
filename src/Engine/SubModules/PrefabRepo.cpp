#include "PrefabRepo.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

redengine::prefab& redengine::PrefabRepo::AddNewPrefab(const std::string& key) {
    prefabMap_.emplace(key, prefab());
    return prefabMap_.at(key);
}

redengine::PrefabRepo::PrefabRepo() {
    JSONLoader::LoadPrefabList();
}
