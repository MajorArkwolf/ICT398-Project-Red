#include "Draw.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Basic.hpp"
#include "Engine/Engine.hpp"
#include <glm/matrix.hpp>


void System::Draw(entt::registry& registry, const glm::mat4& projection, const glm::mat4& view) {
    registry.view<Component::Model, Component::Transform>().each([projection, view](Component::Model &mod, Component::Transform &tran){
        // Generate and configure the GameObject's model matrix
        glm::mat4 model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, tran.pos);
        model_matrix = glm::scale(model_matrix, tran.scale);
        model_matrix = model_matrix * glm::mat4_cast(tran.rot);

        mod.shader->use();
        mod.shader->setMat4("projection", projection);
        mod.shader->setMat4("view", view);
        mod.shader->setMat4("model", model_matrix);

        auto& model_manager = RedEngine::Engine::get().modelManager;
        model_manager.Draw(mod.id, mod.shader.get());
    });
}
