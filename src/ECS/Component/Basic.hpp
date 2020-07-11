#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

struct Position {
    glm::vec3 pos = {0.0f, 0.0f, 0.0f};
    glm::quat rot = {1.0f};
    double scale = 1.0f;
};