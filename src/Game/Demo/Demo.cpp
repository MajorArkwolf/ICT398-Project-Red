#include "Demo.hpp"
#include "Engine/Engine.hpp"

Demo::Demo() {
    camera = Engine::Camera();
    relativeMouse = true;
}

void Demo::Init() {

}

void Demo::UnInit() {

}

void Demo::Display() {
    auto &renderer = RedEngine::Engine::get().renderer;
    renderer.SetCameraOnRender(camera);
}

void Demo::GUIStart() {

}

void Demo::GUIEnd() {

}

void Demo::Update(double t, double dt) {

}

void Demo::FixedUpdate(double t, double dt) {

}
