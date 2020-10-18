#include "PrefabRepo.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

void redengine::prefab::to_json(nlohmann::json& json, const redengine::prefab& fab) {
    json = nlohmann::json();
    json["Name"] = fab.name;
    json["Model"]["ModelFilePath"] = fab.model_dir;
    json["Transform"]["Position"]["X"] = fab.position_local.x;
    json["Transform"]["Position"]["Y"] = fab.position_local.y;
    json["Transform"]["Position"]["Z"] = fab.position_local.z;
    json["Transform"]["Rotation"]["X"] = fab.rotation_local.x;
    json["Transform"]["Rotation"]["Y"] = fab.rotation_local.y;
    json["Transform"]["Rotation"]["Z"] = fab.rotation_local.z;
    json["Transform"]["Scale"] = fab.scale_local.x;
    json["Physics"]["Static"] = fab.is_static;
    json["Physics"]["Colliders"] = {};
    for (const auto& col_body : fab.colliders_) {
        auto col = nlohmann::json();
        col["Name"] = col_body.part_name;
        col["Mass"] = col_body.mass;
        std::string shape = "";
        //TODO: Add variant shenanigans
        json["Physics"]["Colliders"].push_back(col);
    }
}

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

void redengine::PrefabRepo::InsertPrefab(const prefab& new_prefab) {
    prefabMap_.insert({new_prefab.name, new_prefab});
}
