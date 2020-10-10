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

            auto &transform_component = ent->AddComponent<component::Transform>();
            auto transform = GetJsonField(j, "Colliders", "Transform", JsonType::Json);
            if (transform.has_value()) {
                try {
                    auto position_field = GetJsonField(transform->get(), std::string("Colliders"), std::string("Position"), JsonType::Json);
                    auto rotation_field = GetJsonField(transform->get(), std::string("Colliders"), std::string("Rotation"), JsonType::Json);
                    auto scale_field = GetJsonField(transform->get(), file_path.string(), std::string("Scale"), JsonType::Number);

                    if (position_field.has_value()) {
                        auto x_field = GetJsonField(position_field->get(), "Prefab Position X", "X", JsonType::Number);
                        auto y_field = GetJsonField(position_field->get(), "Prefab Position Y", "Y", JsonType::Number);
                        auto z_field = GetJsonField(position_field->get(), "Prefab Position Z", "Z", JsonType::Number);
                        if (x_field.has_value() && y_field.has_value() && z_field.has_value()) {
                            transform_component.pos = {x_field->get().get<float>(), y_field->get().get<float>(), z_field->get().get<float>()};
                        } else {
                            std::stringstream error;
                            error << "File: " << file_path << " Transform: Position does not contain all coordinate fields, aborting read.";
                            console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                        }
                    } else {
                        std::stringstream error;
                        error << "File: " << file_path << " Transform: does not contain \"Position\" field, aborting read.";
                        console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                    }

                    if (scale_field.has_value()) {
                        transform_component.scale = {scale_field->get().get<float>(), scale_field->get().get<float>(), scale_field->get().get<float>()};
                    }

                    if (rotation_field.has_value()) {
                        auto x_field = GetJsonField(rotation_field->get(), "Prefab Position X", "X", JsonType::Number);
                        auto y_field = GetJsonField(rotation_field->get(), "Prefab Position Y", "Y", JsonType::Number);
                        auto z_field = GetJsonField(rotation_field->get(), "Prefab Position Z", "Z", JsonType::Number);
                        if (x_field.has_value() && y_field.has_value() && z_field.has_value()) {
                            transform_component.rot = {glm::dquat(glm::dvec3(glm::radians(x_field->get().get<float>()), glm::radians(y_field->get().get<float>()), glm::radians(z_field->get().get<float>())))};
                        } else {
                            std::stringstream error;
                            error << "File: " << file_path << " Transform: Rotation does not contain all coordinate fields, aborting read.";
                            console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                        }
                    } else {
                        std::stringstream error;
                        error << "File: " << file_path << " Transform: does not contain \"Rotation\" field, aborting read.";
                        console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                    }
                    transform_component.pos += prefab.position_local;
                    transform_component.rot *= prefab.rotation_local;
                    transform_component.scale *= prefab.scale_local;

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

                } catch (const std::exception &e) {
                    std::cerr << "JSON Transform failed: " << e.what() << '\n';
                }
            }

            if (j.contains("Physics") && prefab.has_physics && pw != nullptr) {
                auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
                auto &trans = ent->GetComponent<component::Transform>();
                auto &phys = ent->AddComponent<component::PhysicBody>();
                pw->AddCollisionBody(ent->GetID(), trans.pos, trans.rot);

                for (const auto &n : prefab.colliders_) {
                    std::visit(overload{
                                   [&](std::monostate) {
                                       console_log.AddLog(ConsoleLog::LogType::Collision, "Collider: " + n.part_name + " does not contain a physics shape when trying to instatiate.", __LINE__, __FILE__);
                                   },
                                   [&](redengine::Capsule shape) {
                                       auto capsule = physics_engine.CreateCapsuleShape(shape.radius * trans.scale.x, shape.height * trans.scale.y);
                                       pw->AddCollider(ent->GetID(), capsule, n.position_local * trans.scale, n.rotation_local);
                                   },
                                   [&](redengine::Box shape) {
                                       auto box = physics_engine.CreateBoxShape(shape.extents * trans.scale.x);
                                       pw->AddCollider(ent->GetID(), box, n.position_local * trans.scale.x, n.rotation_local);
                                   },
                                   [&](redengine::Sphere shape) {
                                       auto sphere = physics_engine.CreateSphereShape(shape.radius);
                                       pw->AddCollider(ent->GetID(), sphere, n.position_local, n.rotation_local);
                                   }},
                               n.shape);
                    if (prefab.collision_shapes.find(n.base_shape_name) != prefab.collision_shapes.end()) {
                        auto pfc = prefab.collision_shapes.at(n.base_shape_name);
                        pw->AddCollider(ent->GetID(), pfc, n.position_local * trans.scale, n.rotation_local);
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

                auto transform = GetJsonField(p, "Colliders", "Transform", JsonType::Json);
                if (transform.has_value()) {
                    try {
                        auto position_field = GetJsonField(transform->get(), "Colliders", "Position", JsonType::Json);
                        auto rotation_field = GetJsonField(transform->get(), "Colliders", "Rotation", JsonType::Json);
                        auto scale_field = GetJsonField(transform->get(), prefab_full_path.string(), "Scale", JsonType::Number);

                        if (position_field.has_value()) {
                            auto x_field = GetJsonField(position_field->get(), "Prefab Position X", "X", JsonType::Number);
                            auto y_field = GetJsonField(position_field->get(), "Prefab Position Y", "Y", JsonType::Number);
                            auto z_field = GetJsonField(position_field->get(), "Prefab Position Z", "Z", JsonType::Number);
                            if (x_field.has_value() && y_field.has_value() && z_field.has_value()) {
                                prefab.position_local = {x_field->get().get<float>(), y_field->get().get<float>(), z_field->get().get<float>()};
                            } else {
                                std::stringstream error;
                                error << "File: " << prefab_full_path << " Transform: Position does not contain all coordinate fields, aborting read.";
                                console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                                break;
                            }
                        } else {
                            std::stringstream error;
                            error << "File: " << prefab_full_path << " Transform: does not contain \"Position\" field, aborting read.";
                            console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                            break;
                        }

                        if (scale_field.has_value()) {
                            prefab.scale_local = {scale_field->get().get<float>()};
                        }

                        if (rotation_field.has_value()) {
                            auto x_field = GetJsonField(rotation_field->get(), "Prefab Position X", "X", JsonType::Number);
                            auto y_field = GetJsonField(rotation_field->get(), "Prefab Position Y", "Y", JsonType::Number);
                            auto z_field = GetJsonField(rotation_field->get(), "Prefab Position Z", "Z", JsonType::Number);
                            if (x_field.has_value() && y_field.has_value() && z_field.has_value()) {
                                prefab.rotation_local = {glm::dquat(glm::dvec3(glm::radians(x_field->get().get<float>()), glm::radians(y_field->get().get<float>()), glm::radians(z_field->get().get<float>())))};
                            } else {
                                std::stringstream error;
                                error << "File: " << prefab_full_path << " Transform: Rotation does not contain all coordinate fields, aborting read.";
                                console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                                break;
                            }
                        } else {
                            std::stringstream error;
                            error << "File: " << prefab_full_path << " Transform: does not contain \"Rotation\" field, aborting read.";
                            console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                            break;
                        }
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
                    try {
                        auto colliders = physics.at("Colliders");
                        if (colliders.is_array()) {
                            for (auto &json_collider : colliders) {
                                redengine::Collider collider;

                                auto part_name = GetJsonField(json_collider, "Colliders", "Name", JsonType::String);
                                if (part_name.has_value()) {
                                    collider.part_name = part_name->get().get<std::string>();
                                } else {
                                    std::stringstream error;
                                    error << "File: " << prefab_full_path << " Collider: " << collider.part_name << " does not contain \"Name\" field, aborting read.";
                                    console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                                    break;
                                }

                                auto mass = GetJsonField(json_collider, "Colliders", "Mass", JsonType::Number);
                                if (mass.has_value()) {
                                    collider.mass = json_collider.at("Mass").get<float>();
                                } else {
                                    std::stringstream error;
                                    error << "File: " << prefab_full_path << " Collider: " << collider.part_name << " does not contain \"Mass\" field, defaulting to 1.0kg.";
                                    console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                                    collider.mass = 1.0f;
                                }

                                auto type_field = GetJsonField(json_collider, "Colliders", "Type", JsonType::String);
                                if (type_field.has_value()) {
                                    auto type = type_field->get().get<std::string>();

                                    if (type == "Sphere") {
                                        auto radius_field = GetJsonField(json_collider, "Type: Sphere", "Radius", JsonType::Number);
                                        if (radius_field.has_value()) {
                                            collider.shape = redengine::Sphere({radius_field->get().get<float>()});
                                        }
                                    } else if (type == "Box") {
                                        auto extents_field = GetJsonField(json_collider, "Type: Box", "HalfExtents", JsonType::Json);
                                        if (extents_field.has_value()) {
                                            auto x_field = GetJsonField(extents_field->get(), "Type: Box Extents", "X", JsonType::Number);
                                            auto y_field = GetJsonField(extents_field->get(), "Type: Box Extents", "Y", JsonType::Number);
                                            auto z_field = GetJsonField(extents_field->get(), "Type: Box Extents", "Z", JsonType::Number);
                                            if (x_field.has_value() && y_field.has_value() && z_field.has_value()) {
                                                collider.shape = redengine::Box({{x_field->get().get<float>(), y_field->get().get<float>(), z_field->get().get<float>()}});
                                            }
                                        }
                                    } else if (type == "Capsule") {
                                        auto radius_field = GetJsonField(json_collider, "Type: Capsule", "Radius", JsonType::Number);
                                        auto height_field = GetJsonField(json_collider, "Type: Capsule", "Height", JsonType::Number);
                                        if (radius_field.has_value() && height_field.has_value()) {
                                            collider.shape = redengine::Capsule({radius_field->get().get<float>(), height_field->get().get<float>()});
                                        }
                                    }
                                } else {
                                    std::stringstream error;
                                    error << "File: " << prefab_full_path << " Collider: " << collider.part_name << " does not contain \"Type\" field, aborting read.";
                                    console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                                    break;
                                }

                                prefab.mass += collider.mass;

                                auto com_field = GetJsonField(json_collider, "Colliders", "CentreOfMass", JsonType::Json);
                                if (com_field.has_value()) {
                                    auto x_field = GetJsonField(com_field->get(), "CentreOfMass X", "X", JsonType::Number);
                                    auto y_field = GetJsonField(com_field->get(), "CentreOfMass Y", "Y", JsonType::Number);
                                    auto z_field = GetJsonField(com_field->get(), "CentreOfMass Z", "Z", JsonType::Number);
                                    if (x_field.has_value() && y_field.has_value() && z_field.has_value()) {
                                        collider.centre_of_mass = {x_field->get().get<float>(), y_field->get().get<float>(), z_field->get().get<float>()};
                                    }
                                } else {
                                    std::stringstream error;
                                    error << "File: " << prefab_full_path << " Collider: " << collider.part_name << " does not contain \"CentreOfMass\" field, defaulting to {0,0,0}.";
                                    console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                                    collider.centre_of_mass = {0.f, 0.f, 0.f};
                                }

                                auto position_field = GetJsonField(json_collider, "Colliders", "Position", JsonType::Json);
                                if (position_field.has_value()) {
                                    auto x_field = GetJsonField(position_field->get(), "Position X", "X", JsonType::Number);
                                    auto y_field = GetJsonField(position_field->get(), "Position Y", "Y", JsonType::Number);
                                    auto z_field = GetJsonField(position_field->get(), "Position Z", "Z", JsonType::Number);
                                    if (x_field.has_value() && y_field.has_value() && z_field.has_value()) {
                                        collider.position_local = {x_field->get().get<float>(), y_field->get().get<float>(), z_field->get().get<float>()};
                                    }
                                } else {
                                    std::stringstream error;
                                    error << "File: " << prefab_full_path << " Collider: " << collider.part_name << " does not contain \"Position\" field, defaulting to {0,0,0}.";
                                    console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                                    collider.position_local = {0.f, 0.f, 0.f};
                                }

                                auto rotation_field = GetJsonField(json_collider, "Colliders", "Rotation", JsonType::Json);
                                if (rotation_field.has_value()) {
                                    auto x_field = GetJsonField(rotation_field->get(), "Rotation X", "X", JsonType::Number);
                                    auto y_field = GetJsonField(rotation_field->get(), "Rotation Y", "Y", JsonType::Number);
                                    auto z_field = GetJsonField(rotation_field->get(), "Rotation Z", "Z", JsonType::Number);
                                    if (x_field.has_value() && y_field.has_value() && z_field.has_value()) {
                                        collider.rotation_local = {glm::dquat(glm::dvec3(glm::radians(x_field->get().get<float>()), glm::radians(y_field->get().get<float>()), glm::radians(z_field->get().get<float>())))};
                                    }
                                } else {
                                    std::stringstream error;
                                    error << "File: " << prefab_full_path << " Collider: " << collider.part_name << " does not contain \"Rotation\" field, defaulting to no rotation.";
                                    console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
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
                                    glm::vec3 average = {};
                                    for (auto &n : weighted_collider_centre_mass) {
                                        average += n;
                                    }
                                    return average / static_cast<float>(weighted_collider_centre_mass.size());
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
                          << " does not contain a 'Physics' json object";
                    console_log.AddLog(ConsoleLog::LogType::Collision, error.str(), __LINE__, __FILE__);
                }
                console_log.AddLog(ConsoleLog::LogType::Collision, name + " sucessfully added to prefab list", __LINE__, __FILE__);
            }
        }
    }
}

std::optional<std::reference_wrapper<nlohmann::json>> JSONLoader::GetJsonField(nlohmann::json &input_json, const std::string &json_name, const std::string &field_name, JsonType expectedType) {
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
