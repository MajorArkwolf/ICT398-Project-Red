#include "GUIManager.hpp"
#include "Engine/Engine.hpp"
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

GUIManager::GUIManager() {
    InitialiseWindowOpenMap();
}

GUIManager::~GUIManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUIManager::InitialiseImGUI(GLFWwindow *window) {

    // Setup ImGui.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void GUIManager::DisplayInputRebindWindow() {
    //auto &inputManager = Controller::Input::InputManager::getInstance();
    //auto &inputMap     = inputManager.getInputMap();
    auto &window_open   = window_open_map.at("controls");
    //auto &resManager   = ResourceManager::getInstance();
    //const auto *state        = inputManager.getKeyStates();

    if (window_open) {
//
//        ImGui::Begin(resManager.getString("ControlMenu_title").c_str(), &window_open_map.at("controls"),
//                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
//        ImGui::Text("Click on a button while holding a key to map an action to that key");
//        ImGui::Separator();
//
//        for (auto &n : inputMap) {
//            switch (n.first) {
//                case Controller::Input::BLUE_InputAction::INPUT_DEFAULT:
//                case Controller::Input::BLUE_InputAction::INPUT_LOOK_UP:
//                case Controller::Input::BLUE_InputAction::INPUT_LOOK_DOWN:
//                case Controller::Input::BLUE_InputAction::INPUT_LOOK_LEFT:
//                case Controller::Input::BLUE_InputAction::INPUT_LOOK_RIGHT: {
//                } break;
//                default: {
//                    auto scancodeString = inputManager.hashGLFWKeyToString(n.second);
//                    auto actionString   = inputManager.hashInputActionToString(n.first);
//                    ImGui::Text(actionString.c_str());
//                    ImGui::SameLine(ImGui::GetWindowWidth() - 80);
//
//                    if (ImGui::Button(scancodeString.c_str())) {
//                        const auto &scancodePairs = inputManager.getStringScancodePairs();
//                        for (const auto &i : scancodePairs) {
//                            if (state[i.second]) {
//                                inputManager.bindKey(n.first, i.second);
//                                break;
//                            }
//                        }
//                    }
//
//                } break;
//            }
//        }

        ImGui::End();
    }
}

void GUIManager::DisplayEscapeMenu() {
    // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    bool &window_open = window_open_map.at("menu");
    //auto &resManager = ResourceManager::getInstance();
    auto &engine = redengine::Engine::get();

    if (window_open) {
        ImGui::SetNextWindowPos(ImVec2(0.5, 0.5), ImGuiCond_Always, ImVec2(-0.5, -0.5));
//        ImGui::Begin(resManager.getString("OptionMenu_title").c_str(), &windowOpen,
//                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
//        if (ImGui::Button("Controls")) {
//            ToggleWindow("controls");
//        }
//        if (ImGui::Button("Instructions")) {
//            ToggleWindow("instructions");
//        }
//        if (ImGui::Button("Dev Menu")) {
//            ToggleWindow("dev");
//        }
//        if (ImGui::Button("Textures")) {
//            ToggleWindow("texture");
//        }
//        if (ImGui::Button("Terrain Settings")) {
//            ToggleWindow("terrainSettings");
//        }
//        if (ImGui::Button("Settings")) {
//            engine.show_settings_menu_ = true;
//        }
//        if (ImGui::Button("Exit")) {
//            ToggleWindow("exit");
//        }
        ImGui::End();
    }
}

void GUIManager::DisplayInstructionMenu() {
}

void GUIManager::DisplayQuitScreen() {

}

void GUIManager::DisplayDevScreen(engine::Camera &camera) {
    // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    bool &window_open = window_open_map.at("dev");
    if (window_open) {
        auto key = camera.GetLocation();
        ImGui::Begin("Dev Menu", &window_open, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Camera Position: %f, %f, %f", camera.position_.x, camera.position_.y,
                    camera.position_.z);
        //ImGui::SliderFloat("Camera Speed", &camera.MovementSpeed, 0.001, 2.0);
        ImGui::Text("Camera Location Key: %d, %d", key.x, key.y);
        ImGui::End();
    }
}

void GUIManager::DisplayTextureManager() {
}

void GUIManager::DisplayTerrainSettings() {

}

void GUIManager::startWindowFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUIManager::EndWindowFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::ToggleWindow(const std::string &windowName) {
    if (window_open_map.find(windowName) != window_open_map.end()) {
        bool &open = window_open_map.at(windowName);
        open       = !open;
    }
}

void GUIManager::InitialiseWindowOpenMap() {
    window_open_map.insert(std::make_pair(std::string("menu"), false));
    window_open_map.insert(std::make_pair(std::string("controls"), false));
    window_open_map.insert(std::make_pair(std::string("instructions"), false));
    window_open_map.insert(std::make_pair(std::string("exit"), false));
    window_open_map.insert(std::make_pair(std::string("dev"), false));
    window_open_map.insert(std::make_pair(std::string("texture"), false));
    window_open_map.insert(std::make_pair(std::string("textureRebind"), false));
    window_open_map.insert(std::make_pair(std::string("terrainSettings"), false));
}
