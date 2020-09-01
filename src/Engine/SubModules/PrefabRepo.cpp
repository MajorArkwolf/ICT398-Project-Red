#include "PrefabRepo.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

redengine::prefab& redengine::PrefabRepo::AddNewPrefab(const std::string& key) {
    prefabMap_.emplace(key, prefab());
    return prefabMap_.at(key);
}

redengine::PrefabRepo::PrefabRepo() {

}

void redengine::PrefabRepo::Init() {
    JSONLoader::LoadPrefabList();
}

bool redengine::PrefabRepo::FindPrefab(const std::string& key) {
    return prefabMap_.find(key) != prefabMap_.end();
}

const redengine::prefab &redengine::PrefabRepo::GetPrefab(const std::string &key) const {
    return prefabMap_.at(key);
}
