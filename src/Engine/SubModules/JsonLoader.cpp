#include "JsonLoader.hpp"
#include <nlohmann/json.hpp>
#include "ECS/Ecs.hpp"
#include "ECS/Entity.hpp"
#include <iostream>
#include <fstream>
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "Engine/Renderer/Shader.hpp"

std::optional<std::shared_ptr<Entity>> JSONLoader::LoadEntity(const std::string& filepath, ECS& ecs) {
    using namespace nlohmann;

    std::optional<std::shared_ptr<Entity>> entity = {};

    std::ifstream i(filepath);
    if (!i.is_open()) {
        std::cerr << "ERROR: Load entity " << filepath << " did not load.\n";
        return entity;
    }
    json j = {};
    i >> j;
    entity = std::make_shared<Entity>(ecs.CreateEntity());
    auto &ent = entity.value();
    if (j.contains("Model")) {
        try {
            auto model = j.at("Model");
            auto vertShader = model.at("VertShader").get<std::string>();
            auto fragShader = model.at("FragShader").get<std::string>();
            auto modelFilepath = model.at("ModelFilePath").get<std::string>();
            ent->AddComponent<Component::Model>(modelFilepath,
                                                std::make_shared<Shader>(vertShader.c_str(), fragShader.c_str()));
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
