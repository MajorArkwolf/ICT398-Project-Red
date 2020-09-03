#include <Engine/Renderer/OpenGL.hpp>
#include <Engine/Engine.hpp>
#include "MainMenu.hpp"
#include <imgui.h>
#include "Game/Demo/Demo.hpp"
#include "Game/PhysicsDemo/PhysicsDemo.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

template<class... Ts>
struct overload : Ts... { using Ts::operator()...; };
template<class... Ts>
overload(Ts...) -> overload<Ts...>;

MainMenu::MainMenu() {
    auto &window = redengine::Engine::get().window_;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

MainMenu::~MainMenu() = default;

void MainMenu::Init() {
    camera = engine::Camera();
    camera.position_ = glm::vec3(0.0f, 10.0f, 0.0f);
    std::filesystem::path path = "";
    path.append("MainScreen");
    path.append("Scene.json");
    JSONLoader::LoadScene(path, &ecs, nullptr);
    camera.pitch_ -= 20.0;
    camera.UpdateCameraVectors();
}

auto MainMenu::Display(Shader *shader, const glm::mat4& projection, const glm::mat4& view) -> void {
    auto &engine   = redengine::Engine::get();
    auto &renderer = redengine::Engine::get().renderer_;
    renderer.SetCameraOnRender(camera);
    ecs.Draw(projection, view, camera.GetLocation());
}

auto MainMenu::FixedUpdate(double t, double dt) -> void {
    ecs.FixedUpdate(t, dt);
}

auto MainMenu::Update(double t, double dt) -> void {
    ecs.Update(t, dt);
}

void MainMenu::UnInit() {

}

void MainMenu::HandleWindowEvent() {
    auto &engine = redengine::Engine::get();
    engine.renderer_.ResizeWindow();
}

void MainMenu::HandleInputData(input::InputEvent inputData, double deltaTime) {
    using namespace input;
    auto &engine = redengine::Engine::get();
    auto &gui_manager = engine.GetGuiManager();
    auto handledMouse = false;
    std::visit(overload{
                   [&](std::monostate) {

                   },
                   [&](InputEvent::MouseEvent mouse) {

                   },
                   [&](InputEvent::KeyboardEvent keyboard) {
                       switch (inputData.type) {
                           case input::InputType::kKeyPressed: {
                               switch (keyboard.key) {
                                 
                               }
                           } break;
                           case input::InputType::kKeyReleased: {
                               switch (keyboard.key) {
                                   case input::VirtualKey::kEscape:
                                       gui_manager.ToggleWindow("escapeMenu");
                               }
                           } break;
                       }
                   },
                   [&](InputEvent::dVector2 vec) {

                   },
                   [&](InputEvent::iVector2 vec) {
                      
                   }},
               inputData.data);
    if (!handledMouse) {
        engine.mouse_ = {0.0f, 0.0f};
    }
}

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
    if (ImGui::Button("Physics Demo", ImVec2(285, 40))) {
        engine.game_stack_.AddToStack(std::make_shared<PhysicsDemo>());
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

    engine.GetGuiManager().DisplayEscapeMenu();
    engine.GetGuiManager().DisplayConsoleLog();
}

void MainMenu::GUIEnd() {
    GUIManager::EndWindowFrame();
}
