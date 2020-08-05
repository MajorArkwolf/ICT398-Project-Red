#include "JsonLoader.hpp"

#include <fstream>
#include <iostream>

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Entity.hpp"
#include "ECS/ecs.h"
#include "Engine/Engine.hpp"
#include "Engine/Renderer/Shader.hpp"

using namespace nlohmann;

nlohmann::json JSONLoader::LoadJson(const std::filesystem::path &filepath) {
    json j = {};
    std::ifstream i(filepath);
    if (!i.is_open()) {
        std::cerr << "ERROR: Failed to load JSON file at " << filepath << ".\n";
    }
    i >> j;
    return j;
}

std::optional<std::shared_ptr<Entity>> JSONLoader::LoadEntity(const std::filesystem::path& filepath, ECS& ecs) {
    json j = LoadJson(filepath);
    std::optional<std::shared_ptr<Entity>> entity = {};
    auto basepath = RedEngine::Engine::get().getBasePath();
    auto fullPath = basepath / "res" / "Entity" / filepath;
    entity = std::make_shared<Entity>(ecs.CreateEntity());
    auto &ent = entity.value();
    if (j.contains("Model")) {
        try {
            auto model = j.at("Model");
            auto vertShader = basepath;
            vertShader.append(model.at("VertShader").get<std::string>());
            auto fragShader = basepath;
            fragShader.append(model.at("FragShader").get<std::string>());
            auto modelFilepath = basepath;
            modelFilepath.append(model.at("ModelFilePath").get<std::string>());
            ent->AddComponent<Component::Model>(modelFilepath,
                                                std::make_shared<Shader>(vertShader, fragShader, std::filesystem::path{}));
        } catch (const std::exception& e) {
            std::cerr << "JSON Model failed: " << e.what() << '\n';
        }
    }
    if (j.contains("Transform")) {
        try {
            auto &trans = ent->AddComponent<Component::Transform>();
            auto transform = j.at("Transform");
            auto position = transform.at("Position");
            auto rotation = transform.at("Rotation");
            auto scale = transform.at("Scale");
            trans.pos = {position.at("X").get<float>(), position.at("Y").get<float>(), position.at("Z").get<float>()};
            trans.scale = {scale.at("X").get<float>(), scale.at("Y").get<float>(), scale.at("Z").get<float>()};
            trans.rot = glm::quat(glm::vec3(glm::radians(rotation.at("X").get<float>()), glm::radians(rotation.at("Y").get<float>()),
                         glm::radians(rotation.at("Z").get<float>())));
        } catch (const std::exception& e) {
            std::cerr << "JSON Transform failed: " << e.what() << '\n';
        }
    }
    if (j.contains("Animation")) {
        try {
            auto &anim = ent->AddComponent<Component::Animation>(ent->GetComponent<Component::Model>().id);
            auto idle = j.at("Animation").at("IDLE").get<std::string>();
            anim.animator.LoadAnimation(idle);
        } catch (const std::exception& e) {
            std::cerr << "JSON Animation failed: " << e.what() << '\n';
        }
    }
    return entity;
}

void JSONLoader::LoadScene(const std::filesystem::path &filepath, ECS &ecs) {
    auto basepath = RedEngine::Engine::get().getBasePath();
    auto fullPath = basepath / "res" / "Entity" / filepath;
    auto j = LoadJson(fullPath);
    if (j.contains("Entity")) {
        auto entities = j.at("Entity");
        for (auto &e : entities) {
            auto file = e.get<std::string>();
            auto filename = fullPath.remove_filename().append(file);
            LoadEntity(filename, ecs);
        }
    }
}
