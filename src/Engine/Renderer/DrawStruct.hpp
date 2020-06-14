#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"
#include <vector>
#include <functional>

namespace View::Data {
    struct DrawItem {
        glm::vec3 pos = {};
        float distance = {};
        std::function<void(const glm::mat4&, const glm::mat4&, const glm::dvec3&)> drawPointer;
    };
}