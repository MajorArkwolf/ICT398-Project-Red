#include "GUIManager.hpp"

#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include <sstream>

#include "Engine/Engine.hpp"

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
    auto &input_manager = redengine::Engine::get().input_manager_;
    auto &input_map = input_manager.GetInputMap();
    auto &window_open = window_open_map.at("controls");
    const auto &state = input_manager.GetKeyStates();

    if (window_open) {

        ImGui::Begin("Rebind Controls", &window_open_map.at("controls"),
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::Text("Click on a button while holding a key to map an action to that key");
        ImGui::Separator();

        for (auto &n : input_map) {
            switch (n.first) {
                default: {
                    auto scancodeString = input_manager.HashKeyToString(n.second);
                    auto actionString = input_manager.HashKeyToString(n.first);
                    ImGui::Text(actionString.c_str());
                    ImGui::SameLine(ImGui::GetWindowWidth() - 80);

                    //if (ImGui::Button(scancodeString.c_str())) {
                    //    const auto &scancodePairs = input_manager.GetStringMap();
                    //    for (const auto &i : scancodePairs) {
                    //        if (state[i.second]) {
                    //            input_manager.bindKey(n.first, i.second);
                    //            break;
                    //        }
                    //    }
                    //}

                } break;
            }
        }

        ImGui::End();
    }
}

void GUIManager::DisplayEscapeMenu() {
    // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    bool &window_open = window_open_map.at("escapeMenu");
    auto &engine = redengine::Engine::get();

    if (window_open) {
        ImGui::SetNextWindowPos(ImVec2(0.5, 0.5), ImGuiCond_Always, ImVec2(-0.5, -0.5));
        ImGui::Begin("Escape Menu", &window_open,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::Button("Instructions")) {
            ToggleWindow("instructions");
        }
        if (ImGui::Button("Dev Menu")) {
            ToggleWindow("dev");
        }
        if (ImGui::Button("Toggle Debug Renderer")) {
            renderer_ = !renderer_;
        }
        if (ImGui::Button("Console Log")) {
            ToggleWindow("consoleLog");
        }
        if (ImGui::Button("Settings")) {
            engine.show_settings_menu_ = true;
        }
        if (ImGui::Button("Exit")) {
            engine.CloseScene();
            window_open = !window_open;
        }
        ImGui::End();
    }
}

void GUIManager::DisplayDevScreen(engine::Camera &camera) {
    // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    bool &window_open = window_open_map.at("dev");
    if (window_open) {
        ImGui::Begin("Dev Menu", &window_open, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Camera Position: %f, %f, %f", camera.position_.x, camera.position_.y,
                    camera.position_.z);
        ImGui::SliderFloat("Camera Speed", &camera.movement_speed_, 0.001, 2.0);
        ImGui::End();
    }
}

void GUIManager::DisplayConsoleLog() {

    bool &window_open = window_open_map.at("consoleLog");
    auto &engine = redengine::Engine::get();

    static bool engine_logs = true;
    static bool warning_logs = true;
    static bool error_logs = true;
    static bool collision_logs = true;
    static bool display_file_loc = true;
    static bool time_stamp = true;

    auto GetFile = [&](ConsoleLog::LogLine &log_line) {
        auto file_loc = std::get<3>(log_line).c_str();
        auto line = std::get<2>(log_line);
        if (line != -1 && file_loc != "") {
            char buffer[1000];
            snprintf(&buffer[0], 1000, "[File: \"%s\", Line: %d]", file_loc, line);
            return std::string(buffer);
        }
        return std::string(" ");
    };

    auto PrintLog = [&](ConsoleLog::LogLine log_line) {
        if (time_stamp) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "[%s] ", std::get<4>(log_line).c_str());
            ImGui::SameLine();
        }
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s ", std::get<1>(log_line).c_str());
        if (display_file_loc) {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", GetFile(log_line).c_str());
        }
    };

    if (window_open) {
        ImGui::Begin("Console Log", &window_open,
                     ImGuiWindowFlags_NoCollapse);
        ImGui::Checkbox("Engine Logs", &engine_logs);
        ImGui::SameLine();
        ImGui::Checkbox("Warning Logs", &warning_logs);
        ImGui::SameLine();
        ImGui::Checkbox("Error Logs", &error_logs);
        ImGui::SameLine();
        ImGui::Checkbox("Collision Logs", &collision_logs);
        ImGui::Checkbox("Display Log Location", &display_file_loc);
        ImGui::SameLine();
        ImGui::Checkbox("Display Timestamp", &time_stamp);

        ImGui::Separator();
        ImGui::BeginChild("Scrolling");
        for (auto &n : engine.GetLog().GetLogFile()) {
            switch (std::get<0>(n)) {
                case ConsoleLog::LogType::Engine: {
                    if (engine_logs) {
                        PrintLog(n);
                    }
                } break;
                case ConsoleLog::LogType::Collision: {
                    if (collision_logs) {
                        PrintLog(n);
                    }
                } break;
                case ConsoleLog::LogType::Warning: {
                    if (warning_logs) {
                        PrintLog(n);
                    }
                } break;
                case ConsoleLog::LogType::Error: {
                    if (error_logs) {
                        PrintLog(n);
                    }
                } break;
                default: {
                    PrintLog(n);
                } break;
            }
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHere(1.0f);
        }
        ImGui::EndChild();
        ImGui::End();
    }
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
        open = !open;
    }
}

void GUIManager::InitialiseWindowOpenMap() {
    window_open_map.emplace(std::make_pair(std::string("escapeMenu"), false));
    window_open_map.emplace(std::make_pair(std::string("controls"), false));
    window_open_map.emplace(std::make_pair(std::string("instructions"), false));
    window_open_map.emplace(std::make_pair(std::string("exit"), false));
    window_open_map.emplace(std::make_pair(std::string("dev"), false));
    window_open_map.emplace(std::make_pair(std::string("textureRebind"), false));
    window_open_map.emplace(std::make_pair(std::string("consoleLog"), false));
}
