#include "JsonLoader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "DataStructures/Model/Overload.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Physics/PhysicsEngine.hpp"
#include "Engine/Physics/PhysicsWorld.hpp"

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
    const std::filesystem::path &file_path, ECS *ecs = nullptr, physics::PhysicsWorld *pw) {
    auto &prefabRepo = redengine::Engine::get().GetPrefabRepo();
    auto &console_log = redengine::Engine::get().GetLog();
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
                        scale.get<float>(),
                        scale.get<float>(),
                        scale.get<float>()};
                    trans.rot = glm::quat(glm::vec3(
                        glm::radians(rotation.at("X").get<float>()),
                        glm::radians(rotation.at("Y").get<float>()),
                        glm::radians(rotation.at("Z").get<float>())));
                    trans.pos += prefab.position_local;
                    trans.rot *= prefab.rotation_local;
                    trans.scale *= prefab.scale_local;
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
            } else {
                std::cerr << "ERROR: Prefab was not found during creation of Entity.\n";
            }
            if (j.contains("Physics") && prefab.has_physics && pw != nullptr) {
                auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
                auto &trans = ent->GetComponent<component::Transform>();
                auto &phys = ent->AddComponent<component::PhysicBody>();
                pw->AddCollisionBody(ent->GetID(), trans.pos, trans.rot);

                for (const auto &n : prefab.colliders_) {
                    std::visit(overload{
                                   [&](std::monostate) {
                                       console_log.AddLog(ConsoleLog::LogType::Collision, std::string("Collider: ") + std::string(n.part_name) + std::string(" does not contain a physics shape when trying to instatiate."), __LINE__, __FILE__);
                                   },
                                   [&](redengine::Capsule shape) {
                                       auto capsule = physics_engine.CreateCapsuleShape(shape.radius * trans.scale.x, shape.height * trans.scale.x);
                                       pw->AddCollider(ent->GetID(), capsule, n.position_local * trans.scale.x, n.rotation_local);
                                   },
                                   [&](redengine::Box shape) {
                                       auto box = physics_engine.CreateBoxShape(shape.extents * static_cast<double>(trans.scale.x));
                                       pw->AddCollider(ent->GetID(), box, n.position_local * trans.scale.x , n.rotation_local);
                                   },
                                   [&](redengine::Sphere shape) {
                                       auto sphere = physics_engine.CreateSphereShape(shape.radius);
                                       pw->AddCollider(ent->GetID(), sphere, n.position_local, n.rotation_local);
                                   }},
                               n.shape);
                    if (prefab.collision_shapes.find(n.base_shape_name) != prefab.collision_shapes.end()) {
                        auto pfc = prefab.collision_shapes.at(n.base_shape_name);
                        pw->AddCollider(ent->GetID(), pfc, n.position_local * trans.scale.x, n.rotation_local);
                    } else {
                        ///Prefab name not found
                    }
                }

                phys.colliders = prefab.colliders_;
                phys.mass = prefab.mass;
                phys.centre_mass = prefab.centre_of_mass;
            }
        } else {
            std::cerr << "ERROR: Prefab not specified or was incorrect in Entity creation.\n";
        }
    }
    return entity;
}

void JSONLoader::LoadScene(const std::filesystem::path &file_path, ECS *ecs = nullptr, physics::PhysicsWorld *pw = nullptr) {
    auto base_path = redengine::Engine::get().GetBasePath();
    const auto full_path = base_path / "res" / "Entity" / file_path;
    auto j = LoadJson(full_path);
    if (j.contains("Entity")) {
        auto entities = j.at("Entity");
        for (auto &e : entities) {
            auto file = e.get<std::string>();
            auto file_name = full_path;
            file_name.remove_filename().append(file);
            if (ecs != nullptr) {
                LoadEntity(file_name, ecs, pw);
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
                    auto &e = redengine::Engine::get().GetPhysicsEngine();
                    prefab.has_physics = true;
                    auto physics = p.at("Physics");
                    if (physics.contains("Static")) {
                        prefab.is_static = physics.at("Static").get<bool>();
                    }
                    if (true) {
                        try {
                            auto colliders = physics.at("Colliders");
                            if (colliders.is_array()) {
                                for (auto &json_collider : colliders) {
                                    redengine::Collider collider;

                                    auto part_name = GetJsonField(json_collider, std::string("Colliders"), std::string("Name"), JsonType::String);
                                    if (part_name.has_value()) {
                                        collider.part_name = part_name->get().get<std::string>();
                                    }

                                    auto mass = GetJsonField(json_collider, std::string("Colliders"), std::string("Mass"), JsonType::Number);
                                    if (mass.has_value()) {
                                        collider.mass = json_collider.at("Mass").get<float>();
                                    } else {
                                        collider.mass = 1.0f;
                                    }

                                    auto type_field = GetJsonField(json_collider, std::string("Colliders"), std::string("Type"), JsonType::String);
                                    if (type_field.has_value()) {
                                        auto type = type_field->get().get<std::string>();

                                        if (type == "Sphere") {
                                            auto radius_field = GetJsonField(json_collider, std::string("Type: Sphere"), std::string("Radius"), JsonType::Number);
                                            if (radius_field.has_value()) {
                                                collider.shape = redengine::Sphere({radius_field->get().get<double>()});
                                            }
                                        } else if (type == "Box") {
                                            auto extents_field = GetJsonField(json_collider, std::string("Type: Box"), std::string("HalfExtents"), JsonType::Json);
                                            if (extents_field.has_value()) {
                                                auto x_field = GetJsonField(extents_field->get(), std::string("Type: Box Extents"), std::string("X"), JsonType::Number);
                                                auto y_field = GetJsonField(extents_field->get(), std::string("Type: Box Extents"), std::string("Y"), JsonType::Number);
                                                auto z_field = GetJsonField(extents_field->get(), std::string("Type: Box Extents"), std::string("Z"), JsonType::Number);
                                                if (x_field.has_value() && y_field.has_value() && z_field.has_value()) {
                                                    collider.shape = redengine::Box({{x_field->get().get<double>(), y_field->get().get<double>(), z_field->get().get<double>()}});
                                                }
                                            }
                                        } else if (type == "Capsule") {
                                            auto radius_field = GetJsonField(type_field->get(), std::string("Type: Capsule"), std::string("Radius"), JsonType::Number);
                                            auto height_field = GetJsonField(type_field->get(), std::string("Type: Capsule"), std::string("Height"), JsonType::Number);
                                            if (radius_field.has_value() && height_field.has_value()) {
                                                collider.shape = redengine::Capsule({radius_field->get().get<double>(), height_field->get().get<double>()});
                                            }
                                        }
                                    }

                                    prefab.mass += collider.mass;

                                    if (json_collider.contains("CentreOfMass")) {
                                        auto com = json_collider.at("CentreOfMass");
                                        glm::vec3 vec_com = {com.at("X").get<float>(),
                                                             com.at("Y").get<float>(),
                                                             com.at("Z").get<float>()};
                                        collider.centre_of_mass = vec_com;
                                    } else {
                                        console_log.AddLog(ConsoleLog::LogType::Collision, std::string("Collider: ") + std::string(collider.part_name) + std::string(" does not contain \"CentreOfMass\" field, defaulting to {0,0,0}."), __LINE__, __FILE__);
                                        collider.centre_of_mass = {0.f, 0.f, 0.f};
                                    }

                                    if (json_collider.contains("Position")) {
                                        auto position = json_collider.at("Position");
                                        collider.position_local = {position.at("X").get<float>(),
                                                                   position.at("Y").get<float>(),
                                                                   position.at("Z").get<float>()};
                                    } else {
                                        console_log.AddLog(ConsoleLog::LogType::Collision, std::string("Collider: ") + std::string(collider.part_name) + std::string(" does not contain \"Position\" field, defaulting to {0,0,0}."), __LINE__, __FILE__);
                                        collider.position_local = {0.f, 0.f, 0.f};
                                    }

                                    if (json_collider.contains("Rotation")) {
                                        auto rotation = json_collider.at("Rotation");
                                        collider.rotation_local = glm::quat(glm::vec3(
                                            glm::radians(rotation.at("X").get<float>()),
                                            glm::radians(rotation.at("Y").get<float>()),
                                            glm::radians(rotation.at("Z").get<float>())));

                                    } else {
                                        console_log.AddLog(ConsoleLog::LogType::Collision, std::string("Collider: ") + std::string(collider.part_name) + std::string(" does not contain \"Rotation\" field, defaulting to no rotation."), __LINE__, __FILE__);
                                        collider.rotation_local = {1.0f, 0.f, 0.f, 0.f};
                                    }

                                    prefab.colliders_.push_back(collider);
                                }

                                //Calculate centre of mass for entire prefab
                                {
                                    glm::dvec3 centre_mass{0, 0, 0};
                                    std::vector<glm::dvec3> weighted_collider_centre_mass;
                                    for (auto &n : prefab.colliders_) {
                                        float weight = n.mass / prefab.mass;
                                        weighted_collider_centre_mass.emplace_back(n.centre_of_mass / weight);
                                    }

                                    auto average = std::invoke([&]() {
                                        glm::dvec3 average = {};
                                        for (auto &n : weighted_collider_centre_mass) {
                                            average += n;
                                        }
                                        return average / static_cast<double>(weighted_collider_centre_mass.size());
                                    });
                                    prefab.centre_of_mass = average;
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

std::optional<std::reference_wrapper<nlohmann::json>> JSONLoader::GetJsonField(nlohmann::json &input_json, std::string &json_name, std::string &field_name, JsonType expectedType) {
    using json_ref_type = std::optional<std::reference_wrapper<nlohmann::json>>;
    auto &console_log = redengine::Engine::get().GetLog();
    bool unexpected_type = false;
    if (input_json.contains(field_name)) {
        switch (expectedType) {
            case JsonType::Json: {
                if (!input_json.at(field_name).is_object()) {
                    unexpected_type = true;
                }
            } break;
            case JsonType::Array: {
                if (!input_json.at(field_name).is_array()) {
                    unexpected_type = true;
                }
            } break;
            case JsonType::Float: {
                if (!input_json.at(field_name).is_number_float()) {
                    unexpected_type = true;
                }
            } break;
            case JsonType::Int: {
                if (!input_json.at(field_name).is_number_integer()) {
                    unexpected_type = true;
                }
            } break;
            case JsonType::String: {
                if (!input_json.at(field_name).is_string()) {
                    unexpected_type = true;
                }
            } break;
            case JsonType::Boolean: {
                if (!input_json.at(field_name).is_boolean()) {
                    unexpected_type = true;
                }
            } break;
            case JsonType::Number: {
                if (!input_json.at(field_name).is_number()) {
                    unexpected_type = true;
                }
            } break;
        }
        if (unexpected_type) {
            std::stringstream text;
            text << "Field: \"" << field_name << "\" in Json Object: \"" << json_name << "\" was not the expected type";
            console_log.AddLog(ConsoleLog::LogType::Collision, text.str(), __LINE__, __FILE__);
        } else {
            return json_ref_type{input_json.at(field_name)};
        }
    } else {
        std::stringstream text;
        text << "Could not find \"" << field_name << "\" in Json Object: \"" << json_name << "\"";
        console_log.AddLog(ConsoleLog::LogType::Json, text.str(), __LINE__, __FILE__);
    }
    return std::nullopt;
}
