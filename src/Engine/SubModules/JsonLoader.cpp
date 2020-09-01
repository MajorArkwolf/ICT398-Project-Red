#include "JsonLoader.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Physics/PhysicsEngine.hpp"

nlohmann::json JSONLoader::LoadJson(const std::filesystem::path &file_path) {
    nlohmann::json j = {};
    std::ifstream i(file_path);
    if (!i.is_open()) {
        std::cerr << "ERROR: Failed to load JSON file at " << file_path << ".\n";
    }
    i >> j;
    return j;
}

std::optional<std::shared_ptr<Entity>> JSONLoader::LoadEntity(
        const std::filesystem::path &file_path, ECS *ecs = nullptr, PhysicsEngine *pe = nullptr) {
    auto &prefabRepo = redengine::Engine::get().GetPrefabRepo();
    nlohmann::json j = LoadJson(file_path);
    std::optional<std::shared_ptr<Entity>> entity = {};
    if (ecs == nullptr) {
        return entity;
    }
    auto base_path = redengine::Engine::get().GetBasePath();
    auto full_path = base_path / "res" / "Entity" / file_path;
    if (j.contains("Prefab")) {
        auto prefab_key = j.at("Prefab").get<std::string>();
        if (prefabRepo.FindPrefab(prefab_key)) {
            auto &prefab = prefabRepo.GetPrefab(prefab_key);
            entity = std::make_shared<Entity>(ecs->CreateEntity());
            auto &ent = entity.value();
            ent->AddComponent<component::Model>(prefab.model_id, prefab.model_shader);
            if (j.contains("Transform")) {
                try {
                    auto &trans = ent->AddComponent<component::Transform>();
                    auto transform = j.at("Transform");
                    auto position = transform.at("Position");
                    auto rotation = transform.at("Rotation");
                    auto scale = transform.at("Scale");
                    trans.pos = {
                            position.at("X").get<float>(),
                            position.at("Y").get<float>(),
                            position.at("Z").get<float>()};
                    trans.scale = {
                            scale.at("X").get<float>(),
                            scale.at("Y").get<float>(),
                            scale.at("Z").get<float>()};
                    trans.rot = glm::quat(glm::vec3(
                            glm::radians(rotation.at("X").get<float>()),
                            glm::radians(rotation.at("Y").get<float>()),
                            glm::radians(rotation.at("Z").get<float>())));
                    trans.pos += prefab.position_local;
                    //TODO: Find a way to have a generic rotation and apply ontop of.
                    //trans.rot += prefab.rotation_local;
                    trans.scale += prefab.scale_local;
                } catch (const std::exception &e) {
                    std::cerr << "JSON Transform failed: " << e.what() << '\n';
                }
                if (j.contains("Animation")
                    && entity->get()->HasComponent<component::Model>()) {
                    //TODO: Redo this logic, this is kinda scuffed.
                    try {
                        assert(ent->HasComponent<component::Model>());
                        auto &anim =
                                ent->AddComponent<component::Animation>(
                                        ent->GetComponent<component::Model>().id_);
                        auto idle = j.at("Animation").at("IDLE").get<std::string>();
                        anim.animator_.LoadAnimation(idle);
                    } catch (const std::exception &e) {
                        std::cerr << "JSON Animation failed: " << e.what() << '\n';
                    }
                }
                if (j.contains("Physics") && pe != nullptr) {
                    //TODO implement this
                }
            } else {
                std::cerr << "ERROR: Prefab was not found during creation of Entity.\n";
            }
        } else {
            std::cerr << "ERROR: Prefab not specified or was incorrect in Entity creation.\n";
        }
    }
    return entity;
}

//std::optional<std::shared_ptr<Entity>> JSONLoader::LoadEntity(
//        const std::filesystem::path &file_path, ECS *ecs = nullptr, PhysicsEngine *pe = nullptr) {
//
//    nlohmann::json j = LoadJson(file_path);
//    std::optional<std::shared_ptr<Entity>> entity = {};
//    if (ecs == nullptr) {
//        return entity;
//    }
//    auto base_path = redengine::Engine::get().GetBasePath();
//    auto full_path = base_path / "res" / "Entity" / file_path;
//    entity = std::make_shared<Entity>(ecs->CreateEntity());
//    auto &ent = entity.value();
//    if (j.contains("Model")) {
//        try {
//            auto model = j.at("Model");
//            auto vert_shader = base_path;
//            vert_shader.append(model.at("VertShader").get<std::string>());
//            auto frag_shader = base_path;
//            frag_shader.append(model.at("FragShader").get<std::string>());
//            auto model_file_path = base_path;
//            model_file_path.append(model.at("ModelFilePath").get<std::string>());
//            ent->AddComponent<component::Model>(model_file_path,
//                                                std::make_shared<Shader>(
//                                                        vert_shader,
//                                                        frag_shader,
//                                                        std::filesystem::path{}));
//            assert(entity->get()->HasComponent<component::Model>());
//        } catch (const std::exception &e) {
//            std::cerr << "JSON Model failed: " << e.what() << '\n';
//        }
//    }
//    if (j.contains("Transform")) {
//        try {
//            auto &trans = ent->AddComponent<component::Transform>();
//            auto transform = j.at("Transform");
//            auto position = transform.at("Position");
//            auto rotation = transform.at("Rotation");
//            auto scale = transform.at("Scale");
//            trans.pos = {
//                    position.at("X").get<float>(),
//                    position.at("Y").get<float>(),
//                    position.at("Z").get<float>()};
//            trans.scale = {
//                    scale.at("X").get<float>(),
//                    scale.at("Y").get<float>(),
//                    scale.at("Z").get<float>()};
//            trans.rot = glm::quat(glm::vec3(
//                    glm::radians(rotation.at("X").get<float>()),
//                    glm::radians(rotation.at("Y").get<float>()),
//                    glm::radians(rotation.at("Z").get<float>())));
//        } catch (const std::exception &e) {
//            std::cerr << "JSON Transform failed: " << e.what() << '\n';
//        }
//    }
//    if (j.contains("Animation")
//        && entity->get()->HasComponent<component::Model>()) {
//        try {
//            assert(ent->HasComponent<component::Model>());
//            auto &anim =
//                    ent->AddComponent<component::Animation>(
//                            ent->GetComponent<component::Model>().id_);
//            auto idle = j.at("Animation").at("IDLE").get<std::string>();
//            anim.animator_.LoadAnimation(idle);
//        } catch (const std::exception &e) {
//            std::cerr << "JSON Animation failed: " << e.what() << '\n';
//        }
//    }
//    if (j.contains("Physics") && pe != nullptr) {
//        //TODO implement this
//    }
//    return entity;
//}

void JSONLoader::LoadScene(const std::filesystem::path &file_path, ECS *ecs = nullptr, PhysicsEngine *pe = nullptr) {
    auto base_path = redengine::Engine::get().GetBasePath();
    auto full_path = base_path / "res" / "Entity" / file_path;
    auto j = LoadJson(full_path);
    if (j.contains("Entity")) {
        auto entities = j.at("Entity");
        for (auto &e : entities) {
            auto file = e.get<std::string>();
            auto file_name = full_path.remove_filename().append(file);
            if (ecs != nullptr) {
                LoadEntity(file_name, ecs);
            }
        }
    }
}

void JSONLoader::LoadPrefabList() {
    auto &engine = redengine::Engine::get();
    auto base_path = redengine::Engine::get().GetBasePath();
    auto &prefabRepo = redengine::Engine::get().GetPrefabRepo();
    auto &shaderRepo = redengine::Engine::get().GetShaderRepo();
    auto full_path = base_path / "res" / "prefab";
    auto prefab_list_path = full_path / "prefablist.json";
    auto j = LoadJson(prefab_list_path);
    if (j.contains("Prefab")) {
        auto prefabs = j.at("Prefab");
        for (auto &prefab_path : prefabs) {
            auto prefab_full_path = full_path / prefab_path;
            auto p = LoadJson(prefab_full_path);
            if (p.contains("Name")) {
                auto &prefab = prefabRepo.AddNewPrefab(p.at("Name").get<std::string>());
                prefab.name = p.at("Name").get<std::string>();
                if (p.contains("Model")) {
                    prefab.has_model = true;
                    auto model_file_path = base_path;
                    auto shader_file_path = base_path;
                    model_file_path.append(p.at("Model").at("ModelFilePath").get<std::string>());
                    shader_file_path.append(p.at("Model").at("ShaderFilePath").get<std::string>());
                    prefab.model_id = engine.model_manager_.GetModelID(model_file_path);
                    prefab.model_shader = shaderRepo.ImportShader(shader_file_path);
                }
                if (p.contains("Transform")) {
                    try {
                        auto transform = p.at("Transform");
                        auto position = transform.at("Position");
                        auto rotation = transform.at("Rotation");
                        auto scale = transform.at("Scale");
                        prefab.position_local = {
                                position.at("X").get<float>(),
                                position.at("Y").get<float>(),
                                position.at("Z").get<float>()};
                        prefab.scale_local = {
                                scale.at("X").get<float>(),
                                scale.at("Y").get<float>(),
                                scale.at("Z").get<float>()};
                        prefab.rotation_local = glm::quat(glm::vec3(
                                glm::radians(rotation.at("X").get<float>()),
                                glm::radians(rotation.at("Y").get<float>()),
                                glm::radians(rotation.at("Z").get<float>())));
                    } catch (const std::exception &e) {
                        std::cerr << "JSON Transform failed: " << e.what() << '\n';
                    }
                }
                if (p.contains("Physics")) {
                    auto physics = p.at("Physics");
                }
            }
        }
    }
}
