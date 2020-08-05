#include "GUIManager.hpp"
#include "Engine/Engine.hpp"
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

GUIManager::GUIManager() {
    initialiseWindowOpenMap();
}

GUIManager::~GUIManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUIManager::initialiseImGUI(GLFWwindow *window) {

    // Setup ImGui.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void GUIManager::displayInputRebindWindow() {
    //auto &inputManager = Controller::Input::InputManager::getInstance();
    //auto &inputMap     = inputManager.getInputMap();
    auto &windowOpen   = windowOpenMap.at("controls");
    //auto &resManager   = ResourceManager::getInstance();
    //const auto *state        = inputManager.getKeyStates();

    if (windowOpen) {
//
//        ImGui::Begin(resManager.getString("ControlMenu_title").c_str(), &windowOpenMap.at("controls"),
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

void GUIManager::displayEscapeMenu() {
    // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    bool &windowOpen = windowOpenMap.at("menu");
    //auto &resManager = ResourceManager::getInstance();
    auto &engine = redengine::Engine::get();

    if (windowOpen) {
        ImGui::SetNextWindowPos(ImVec2(0.5, 0.5), ImGuiCond_Always, ImVec2(-0.5, -0.5));
//        ImGui::Begin(resManager.getString("OptionMenu_title").c_str(), &windowOpen,
//                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
//        if (ImGui::Button("Controls")) {
//            toggleWindow("controls");
//        }
//        if (ImGui::Button("Instructions")) {
//            toggleWindow("instructions");
//        }
//        if (ImGui::Button("Dev Menu")) {
//            toggleWindow("dev");
//        }
//        if (ImGui::Button("Textures")) {
//            toggleWindow("texture");
//        }
//        if (ImGui::Button("Terrain Settings")) {
//            toggleWindow("terrainSettings");
//        }
//        if (ImGui::Button("Settings")) {
//            engine.show_settings_menu_ = true;
//        }
//        if (ImGui::Button("Exit")) {
//            toggleWindow("exit");
//        }
        ImGui::End();
    }
}

void GUIManager::displayInstructionMenu() {
}

void GUIManager::displayQuitScreen() {

}

void GUIManager::displayDevScreen(Engine::Camera &camera) {
    // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    bool &windowOpen = windowOpenMap.at("dev");
    if (windowOpen) {
        auto key = camera.getLocation();
        ImGui::Begin("Dev Menu", &windowOpen, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Camera Position: %f, %f, %f", camera.Position.x, camera.Position.y,
                    camera.Position.z);
        //ImGui::SliderFloat("Camera Speed", &camera.MovementSpeed, 0.001, 2.0);
        ImGui::Text("Camera Location Key: %d, %d", key.x, key.y);
        ImGui::End();
    }
}

void GUIManager::displayTextureManager() {
}

void GUIManager::displayTerrainSettings() {

}

void GUIManager::startWindowFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUIManager::endWindowFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::toggleWindow(const std::string &windowName) {
    if (windowOpenMap.find(windowName) != windowOpenMap.end()) {
        bool &open = windowOpenMap.at(windowName);
        open       = !open;
    }
}

void GUIManager::initialiseWindowOpenMap() {
    windowOpenMap.insert(std::make_pair(std::string("menu"), false));
    windowOpenMap.insert(std::make_pair(std::string("controls"), false));
    windowOpenMap.insert(std::make_pair(std::string("instructions"), false));
    windowOpenMap.insert(std::make_pair(std::string("exit"), false));
    windowOpenMap.insert(std::make_pair(std::string("dev"), false));
    windowOpenMap.insert(std::make_pair(std::string("texture"), false));
    windowOpenMap.insert(std::make_pair(std::string("textureRebind"), false));
    windowOpenMap.insert(std::make_pair(std::string("terrainSettings"), false));
}
