#include <Engine/Renderer/OpenGL.hpp>
#include <Engine/Engine.hpp>
#include "MainMenu.hpp"
#include "DataStructures/Model/Animation.hpp"
#include <imgui.h>
#include "Engine/SubModules/GUIManager.hpp"
#include "Game/Demo/Demo.hpp"

//using Controller::Input::BLUE_InputAction;
//using Controller::Input::BLUE_InputType;

MainMenu::MainMenu() {
    auto &window = RedEngine::Engine::get().window;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

MainMenu::~MainMenu() = default;

void MainMenu::Init() {
    camera = Engine::Camera();
    camera.Position = glm::vec3(0.0f, 10.0f, 0.0f);
    auto basepath = RedEngine::Engine::get().getBasePath();
    sModels.emplace_back(basepath / "res" / "model" / "ClothedMan.gltf");
    sModels.emplace_back(basepath / "res" / "model" / "ClothedMan.gltf");
    sModels.at(0).position.x = 20.0f;
    sModels.at(0).position.z = 2.0f;
    sModels.at(0).animator = std::make_shared<Model::Animator>();
    sModels.at(0).animator->LinkToModel(static_cast<unsigned>(sModels.at(0).getModel()));
    sModels.at(0).animator->LoadAnimation("PUNCH");
    sModels.at(0).rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f) ,0.0f));
    sModels.at(1).position.x = 20.5f;
    sModels.at(1).position.z = -2.0f;
    sModels.at(1).animator = std::make_shared<Model::Animator>();
    sModels.at(1).animator->LinkToModel(static_cast<unsigned>(sModels.at(1).getModel()));
    sModels.at(1).animator->LoadAnimation("PUNCH");
    sModels.at(1).rotation = glm::quat(glm::vec3(0.0f, 0.0f ,0.0f));
    camera.Pitch -= 20.0;
    camera.updateCameraVectors();
}

auto MainMenu::Display(glm::mat4 projection, glm::mat4 view) -> void {
    auto &engine   = RedEngine::Engine::get();
    auto &renderer = RedEngine::Engine::get().renderer;
    renderer.SetCameraOnRender(camera);
    for (auto& m : sModels) {
        m.Draw(projection, view);
    }
}

auto MainMenu::FixedUpdate(double t, double dt) -> void {}

auto MainMenu::Update(double t, double dt) -> void {
    for (auto & m : sModels) {
        m.Update(t, dt);
    }
}

void MainMenu::UnInit() {

}

void MainMenu::handleWindowEvent() {
    auto &engine = RedEngine::Engine::get();
    engine.renderer.ResizeWindow();
}

//void MainMenu::handleInputData(Controller::Input::InputData inputData, double deltaTime) {
//    auto &engine      = RedEngine::Engine::get();
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
//            if (!engine.getMouseMode()) {
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
    auto &engine  = RedEngine::Engine::get();
    GUIManager::startWindowFrame();
    MainMenuGUI();
}

void MainMenu::MainMenuGUI() {
    auto &engine = RedEngine::Engine::get();
    ImGui::SetNextWindowSize(ImVec2(300, 500), 1);
    //ImGui::SetNextWindowPosCenter(1);

    ImGui::Begin("Menu", nullptr,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::Separator();
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
    ImGui::Text("Project Blue: Run and Gun");
    if (ImGui::Button("Demo", ImVec2(285, 40))) {
        auto &engine = RedEngine::Engine::get();
        engine.gameStack.AddToStack(std::make_shared<Demo>());
    }
    ImGui::Separator();

    ImGui::Text("Other options");
    if (ImGui::Button("Settings", ImVec2(285, 40))) {
        engine.showSettingsMenu = !engine.showSettingsMenu;
    }
    if (ImGui::Button("Quit", ImVec2(285, 40))) {
        engine.endEngine();
    }

    if (engine.showSettingsMenu) {
        engine.SettingMenu();
    }

    ImGui::End();
}

void MainMenu::GUIEnd() {
    GUIManager::endWindowFrame();
}
