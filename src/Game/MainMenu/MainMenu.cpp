#include <Engine/Renderer/OpenGL.hpp>
#include <Engine/Engine.hpp>
#include "MainMenu.hpp"
#include "DataStructures/Model/Animation.hpp"
#include <imgui.h>
#include "Engine/SubModules/GUIManager.hpp"
#include "Game/Demo/Demo.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

//using Controller::Input::BLUE_InputAction;
//using Controller::Input::BLUE_InputType;

MainMenu::MainMenu() {
    auto &window = redengine::Engine::get().window_;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

MainMenu::~MainMenu() = default;

void MainMenu::Init() {
    camera = Engine::Camera();
    camera.Position = glm::vec3(0.0f, 10.0f, 0.0f);
    auto basepath = redengine::Engine::get().GetBasePath();
    JSONLoader::LoadScene("MainScreen/Scene.json", ecs);
    camera.Pitch -= 20.0;
    camera.updateCameraVectors();
}

auto MainMenu::Display(const glm::mat4& projection, const glm::mat4& view) -> void {
    auto &engine   = redengine::Engine::get();
    auto &renderer = redengine::Engine::get().renderer_;
    renderer.SetCameraOnRender(camera);
    ecs.Draw(projection, view, camera.getLocation());
}

auto MainMenu::FixedUpdate(double t, double dt) -> void {
    ecs.FixedUpdate(t, dt);
}

auto MainMenu::Update(double t, double dt) -> void {
    ecs.Update(t, dt);
}

void MainMenu::UnInit() {

}

void MainMenu::handleWindowEvent() {
    auto &engine = redengine::Engine::get();
    engine.renderer_.ResizeWindow();
}

//void MainMenu::handleInputData(Controller::Input::InputData inputData, double deltaTime) {
//    auto &engine      = redengine::Engine::get();
//    auto &guiManager  = engine.getGuiManager();
//    auto handledMouse = false;
//
//    switch (inputData.inputType) {
//        case BLUE_InputType::KEY_PRESS: { //  Key Press events
//
//            switch (inputData.inputAction) {
//                case BLUE_InputAction::INPUT_MOVE_FORWARD: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_BACKWARD: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_LEFT: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_RIGHT: {
//                } break;
//                default: break;
//            }
//        } break;
//        case BLUE_InputType::KEY_RELEASE: { // Key Release events
//            switch (inputData.inputAction) {
//                case BLUE_InputAction::INPUT_MOVE_FORWARD: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_BACKWARD: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_LEFT: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_RIGHT: {
//                } break;
//                case BLUE_InputAction::INPUT_JUMP: {
//                } break;
//                case BLUE_InputAction::INPUT_CROUCH: {
//                } break;
//                case BLUE_InputAction::INPUT_ACTION_2: {
//                } break;
//                case BLUE_InputAction::INPUT_ACTION_3: {
//                } break;
//                case BLUE_InputAction::INPUT_ACTION_4: {
//                } break;
//                default: break;
//            }
//        } break;
//        case BLUE_InputType::MOUSE_MOTION: { // Mouse motion event
//            if (!engine.GetMouseMode()) {
////                 auto x = static_cast<double>(inputData.mouseMotionRelative.x);
////                 auto y = static_cast<double>(inputData.mouseMotionRelative.y);
////                 y      = y * -1.0;
////                 camera.ProcessMouseMovement(x, y);
////                 handledMouse = true;
//            }
//        } break;
//        case BLUE_InputType::MOUSE_WHEEL: { // Mouse Wheel event
//            auto amountScrolledY = static_cast<double>(inputData.mouseWheelMotion);
//            //camera.ProcessMouseScroll(amountScrolledY);
//        } break;
//        case BLUE_InputType::WINDOW_RESIZE: {
//            this->handleWindowEvent();
//        } break;
//        default: break;
//    }
//    if (!handledMouse) {
//        engine.mouse = {0.0f, 0.0f};
//    }
//}

void MainMenu::GUIStart() {
    auto &engine  = redengine::Engine::get();
    GUIManager::startWindowFrame();
    MainMenuGUI();
}

void MainMenu::MainMenuGUI() {
    auto &engine = redengine::Engine::get();
    ImGui::SetNextWindowSize(ImVec2(300, 500), 1);
    //ImGui::SetNextWindowPosCenter(1);

    ImGui::Begin("Menu", nullptr,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::Separator();
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
    ImGui::Text("Project Blue: Run and Gun");
    if (ImGui::Button("Demo", ImVec2(285, 40))) {
        engine.game_stack_.AddToStack(std::make_shared<Demo>());
    }
    ImGui::Separator();

    ImGui::Text("Other options");
    if (ImGui::Button("Settings", ImVec2(285, 40))) {
        engine.show_settings_menu_ = !engine.show_settings_menu_;
    }
    if (ImGui::Button("Quit", ImVec2(285, 40))) {
        engine.EndEngine();
    }

    if (engine.show_settings_menu_) {
        engine.SettingMenu();
    }

    ImGui::End();
}

void MainMenu::GUIEnd() {
    GUIManager::endWindowFrame();
}
