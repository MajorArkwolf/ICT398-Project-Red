#include <entt/entt.hpp>
#include <glm/matrix.hpp>

namespace System {
    void Draw(entt::registry& registry, const glm::mat4& projection, const glm::mat4& view);
    void UpdateAnimation(entt::registry& registry, double t, double dt);
}
