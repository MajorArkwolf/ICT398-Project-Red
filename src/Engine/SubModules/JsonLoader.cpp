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
            ent->AddComponent<component::Model>(prefab.model_id);
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
                    trans.rot *= prefab.rotation_local;
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
                if (prefab.has_physics && pe != nullptr) {
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
                LoadEntity(file_name, ecs, pe);
            }
        }
    }
}

void JSONLoader::LoadPrefabList() {
    auto &engine = redengine::Engine::get();
    auto base_path = redengine::Engine::get().GetBasePath();
    auto &prefabRepo = redengine::Engine::get().GetPrefabRepo();
    auto full_path = base_path / "res" / "prefab";
    auto prefab_list_path = full_path / "prefablist.json";
    auto &console_log = engine.GetLog();
    auto j = LoadJson(prefab_list_path);
    std::string name = "Unknown";
    if (j.contains("Prefab")) {
        auto prefabs = j.at("Prefab");
        for (auto &prefab_path : prefabs) {
            std::string temp = prefab_path;
            auto prefab_full_path = full_path / temp;
            auto p = LoadJson(prefab_full_path);
            if (p.contains("Name")) {
                auto &prefab = prefabRepo.AddNewPrefab(p.at("Name").get<std::string>());
                name = p.at("Name").get<std::string>();
                prefab.name = name;
                if (p.contains("Model")) {
                    prefab.has_model = true;
                    std::filesystem::path m_path = p.at("Model").at("ModelFilePath").get<std::string>();
                    auto model_file_path = base_path / m_path;
                    prefab.model_id = engine.model_manager_.GetModelID(model_file_path);
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
                    auto e = PhysicsEngine();
                    prefab.has_physics = true;
                    auto physics = p.at("Physics");
                    if (physics.contains("Static")) {
                        prefab.is_static = physics.at("Static").get<bool>();
                    }
                    if (physics.contains("BaseShapes")) {
                        try {
                            auto base_shapes = physics.at("BaseShapes");
                            if (base_shapes.is_array()) {
                                for (auto &element : base_shapes) {
                                    if (element.contains("Name") && element.contains("Type")) {
                                        auto shape_name = element.at("Name").get<std::string>();
                                        if (element.at("Type").get<std::string>() == "Box") {
                                            auto extents = element.at("HalfExtents");
                                            prefab.collision_shapes.emplace(shape_name, e.CreateBoxShape(glm::vec3(extents.at("X").get<float>(), extents.at("Y").get<float>(), extents.at("Z").get<float>())));
                                        } else if (element.at("Type").get<std::string>() == "Capsule") {
                                            auto radius = element.at("Radius").get<float>();
                                            prefab.collision_shapes.emplace(shape_name, e.CreateSphereShape(radius));
                                        } else if (element.at("Type").get<std::string>() == "Sphere") {
                                            auto radius = element.at("Radius").get<float>();
                                            auto height = element.at("Height").get<float>();
                                            prefab.collision_shapes.emplace(shape_name, e.CreateCapsuleShape(radius, height));
                                        }

                                    } else {
                                        console_log.AddLog(ConsoleLog::LogType::Collision, "Base shape does not contain both \"Name\" and \"Type\" fields", __LINE__, __FILE__);
                                    }
                                }
                            }
                            auto colliders = physics.at("Colliders");
                            if (colliders.is_array()) {
                                for (auto &n : colliders) {
                                    redengine::Collider collider;
                                    if (n.contains("Name")) {
                                        collider.part_name = n.at("Name").get<std::string>();
                                    } else {
                                        console_log.AddLog(ConsoleLog::LogType::Collision, "Collider does not contain \"Name\" field", __LINE__, __FILE__);
                                    }
                                    if (n.contains("Name")) {
                                        collider.part_name = n.at("Name").get<std::string>();
                                    } else {
                                        console_log.AddLog(ConsoleLog::LogType::Collision, std::string("Collider: ") + std::string(collider.part_name) + std::string(" does not contain \"BaseShape\" field"), __LINE__, __FILE__);
                                    }
                                    if (n.contains("Mass")) {
                                        collider.mass = n.at("Mass").get<float>();
                                    } else {
                                        collider.mass = 1.0f;
                                    }
                                    if (n.contains("CentreOfMass")) {
                                        auto com = n.at("CentreOfMass");
                                        glm::vec3 vec_com = {com.at("X").get<float>(),
                                                             com.at("Y").get<float>(),
                                                             com.at("Z").get<float>()};
                                        collider.centre_of_mass = vec_com;
                                    } else {
                                        collider.centre_of_mass = {0.f, 0.f, 0.f};
                                    }
                                    if (n.contains("Position")) {
                                        auto position = n.at("Position");
                                        collider.position_local = {position.at("X").get<float>(),
                                                                   position.at("Y").get<float>(),
                                                                   position.at("Z").get<float>()};
                                    } else {
                                        collider.position_local = {0.f, 0.f, 0.f};
                                    }
                                    if (n.contains("Rotation")) {
                                        auto rotation = n.at("Rotation");
                                        collider.rotation_local = glm::quat(glm::vec3(
                                            glm::radians(rotation.at("X").get<float>()),
                                            glm::radians(rotation.at("Y").get<float>()),
                                            glm::radians(rotation.at("Z").get<float>())));

                                    } else {
                                        collider.rotation_local = {1.0f, 0.f, 0.f, 0.f};
                                    }

                                    prefab.colliders_.push_back(collider);
                                }
                            }
                        } catch (const std::exception &e) {
                            std::cerr << "BaseShapes does not contain all correct readable information: " << e.what() << '\n';
                        }
                    } else {
                        std::stringstream error;
                        error << name
                              << " does not contain a 'BaseShapes' array";
                        console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                    }
                    console_log.AddLog(ConsoleLog::LogType::Collision, std::string(name) + std::string(" sucessfully added to prefab list"), __LINE__, __FILE__);
                }
            }
        }
    }
}
