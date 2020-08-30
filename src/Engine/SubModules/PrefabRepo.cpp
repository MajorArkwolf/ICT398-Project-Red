#include "PrefabRepo.hpp"

redengine::prefab& redengine::PrefabRepo::AddNewPrefab() {
    prefab_list.emplace_back();
    return prefab_list.at(prefab_list.size() - 1);
}
