#pragma once

#include <string>
#include <vector>

#include <glm/matrix.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Vertex {
  glm::vec3 position = {};
  glm::vec3 bit_angent = {};
  glm::vec3 tangent = {};
  glm::vec2 normal = {};
  glm::vec2 tex_coords = {};
  glm::ivec4 bone_ids = {};
  glm::vec4 bone_weight = {};
};

struct TextureB {
  std::string type;
  std::string path;
  unsigned int id = 0;
};

struct JointsName {
  std::string name = "";
  glm::mat4 transform = {};
  std::vector<JointsName> children = {};
};

struct BoneInfo {
  glm::mat4 bone_offset = glm::mat4(1.0f);
  glm::mat4 final_transformation = glm::mat4(1.0f);
};
