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
        auto index = col_body.shape.index();
        if (index == 1) {
            auto &cap = std::get<redengine::Sphere>(col_body.shape);
            col["Type"] = "Sphere";
            col["Radius"] = cap.radius;
        } else if (index == 2) {
            auto &cap = std::get<redengine::Box>(col_body.shape);
            col["Type"] = "Box";
            col["HalfExtents"]["X"] = cap.extents.x;
            col["HalfExtents"]["Y"] = cap.extents.y;
            col["HalfExtents"]["Z"] = cap.extents.z;
        } else if (index == 3) {
            auto &cap = std::get<redengine::Capsule>(col_body.shape);
            col["Type"] = "Capsule";
            col["Radius"] = cap.radius;
            col["Height"] = cap.height;
        }
        col["Position"]["X"] = col_body.position_local.x;
        col["Position"]["Y"] = col_body.position_local.y;
        col["Position"]["Z"] = col_body.position_local.z;
        auto eulerRot = glm::degrees(glm::eulerAngles(col_body.rotation_local));
        col["Rotation"]["X"] = eulerRot.x;
        col["Rotation"]["Y"] = eulerRot.y;
        col["Rotation"]["Z"] = eulerRot.z;
        col["Mass"] = col_body.mass;
        col["CentreOfMass"]["X"] = col_body.centre_of_mass.x;
        col["CentreOfMass"]["Y"] = col_body.centre_of_mass.y;
        col["CentreOfMass"]["Z"] = col_body.centre_of_mass.z;
        json["Physics"]["Colliders"].push_back(col);
    }
}

std::vector<std::string> redengine::prefab::GetColliderList() {
    auto list = std::vector<std::string>();
    for (auto &e : colliders_) {
        list.emplace_back(e.part_name);
    }
    return list;
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
    prefabMap_.erase(new_prefab.name);
    prefabMap_.insert({new_prefab.name, new_prefab});
}
