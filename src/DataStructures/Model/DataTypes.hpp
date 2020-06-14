#pragma once
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position = {};
    glm::vec3 Bitangent = {};
    glm::vec3 Tangent = {};
    glm::vec2 Normal = {};
    glm::vec2 TexCoords = {};
    glm::ivec4 BoneIDs = {};
    glm::vec4 BoneWeight = {};
};

struct TextureB {
    std::string type;
    std::string path;
    unsigned int id = 0;
};

struct JointsName {
    std::string name = "";
    glm::mat4 Transform = {};
    std::vector<JointsName> children = {};
};

struct BoneInfo
{
    glm::mat4 BoneOffset = glm::mat4(1.0f);
    glm::mat4 FinalTransformation = glm::mat4(1.0f);
};
