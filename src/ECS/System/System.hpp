#include <entt/entt.hpp>
#include <glm/matrix.hpp>

class Shader;

namespace System {
    void Draw(entt::registry& registry, Shader *shader, const glm::mat4& projection, const glm::mat4& view);
    void UpdateAnimation(entt::registry& registry, double t, double dt);
    void UpdateColors(entt::registry& registry);
}
