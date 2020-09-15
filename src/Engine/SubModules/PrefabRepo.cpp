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

redengine::prefab &redengine::PrefabRepo::GetPrefabMut(const std::string &key) {
    return prefabMap_.at(key);
}

std::vector<std::string> redengine::PrefabRepo::GetPrefabList() {
    auto list = std::vector<std::string>();
    for (auto &e : prefabMap_) {
        list.emplace_back(e.first);
    }
    return list;
}
