#include "MainMenu.hpp"

#include <imgui.h>

#include <Engine/Engine.hpp>
#include <Engine/Renderer/OpenGL.hpp>

#include "Engine/SubModules/JsonLoader.hpp"
#include "Game/Demo/Demo.hpp"
#include "Game/NPCDemo/NPCDemo.hpp"
#include "Game/PhysicsDemo/PhysicsDemo.hpp"
#include "Game/PrefabEditor/PrefabEditor.hpp"
#include "DataStructures/Model/Overload.hpp"

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

auto MainMenu::Display(Shader *shader, const glm::mat4 &projection, const glm::mat4 &view) -> void {
    auto &engine = redengine::Engine::get();
    auto &renderer = redengine::Engine::get().renderer_;
    renderer.SetCameraOnRender(camera);
    ecs.Draw(shader, projection, view);
}

auto MainMenu::FixedUpdate(double t, double dt) -> void {
    ecs.FixedUpdate(t, dt);
}

auto MainMenu::Update(double t, double dt) -> void {
    auto &renderer = redengine::Engine::get().renderer_;
    renderer.SetCameraOnRender(camera);
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
    auto &engine = redengine::Engine::get();
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
    if (ImGui::Button("Basic Demo", ImVec2(285, 40))) {
        auto p = std::make_shared<Demo>();
        p->Init();
        engine.game_stack_.AddToStack(p);
    }
    if (ImGui::Button("NPC AI Demo", ImVec2(285, 40))) {
        auto p = std::make_shared<NPCDemo>();
        p->Init();
        engine.game_stack_.AddToStack(p);
    }
    if (ImGui::Button("Physics Demo", ImVec2(285, 40))) {
        auto p = std::make_shared<PhysicsDemo>();
        p->Init();
        engine.game_stack_.AddToStack(p);
    }
    if (ImGui::Button("Prefab Editor", ImVec2(285, 40))) {
        auto p = std::make_shared<PrefabEditor>();
        p->Init();
        engine.game_stack_.AddToStack(p);
    }
    ImGui::Separator();

    ImGui::Text("Other options");
    if (ImGui::Button("Settings", ImVec2(285, 40))) {
        engine.show_settings_menu_ = !engine.show_settings_menu_;
    }
    if (ImGui::Button("Quit", ImVec2(285, 40))) {
        engine.GetGuiManager().ToggleWindow("quitScreen");
    }

    if (engine.show_settings_menu_) {
        engine.SettingMenu();
    }

    ImGui::End();

    engine.GetGuiManager().DisplayQuitScreen();
}

void MainMenu::GUIEnd() {
    GUIManager::EndWindowFrame();
}
