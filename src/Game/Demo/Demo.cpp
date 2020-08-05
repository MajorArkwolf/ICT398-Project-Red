#include "ECS/Entity.hpp"
#include "Demo.hpp"
#include "Engine/Engine.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Basic.hpp"

#include <iostream>
// TEST
#include "Engine/Engine.hpp"

Demo::Demo() {
    camera = Engine::Camera();
    camera.Position = glm::vec3(0.0f, 10.0f, 0.0f);
    relativeMouse = true;
}

void Demo::Init() {

}

void Demo::UnInit() {

}

void Demo::Display(glm::mat4 projection, glm::mat4 view) {
    auto &renderer = RedEngine::Engine::get().renderer;
    renderer.SetCameraOnRender(camera);
    ecs.Draw(projection, view, camera.getLocation());
}

void Demo::GUIStart() {

}

void Demo::GUIEnd() {

}

void Demo::Update(double t, double dt) {
    ecs.Update(t, dt);
}

void Demo::FixedUpdate(double t, double dt) {
    ecs.FixedUpdate(t, dt);
}

void Demo::HandleInputData(input::InputEvent inputData, double deltaTime)
{
    switch(inputData.type)
    {
        case input::InputType::kButtonPressed :
        {
            std::cout << "asd";
        }
    }
}
