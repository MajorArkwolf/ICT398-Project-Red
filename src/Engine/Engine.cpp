#include "Engine/Engine.hpp"
#include "Engine/SubModules/Input/GLFWInputWrangler.hpp"
#include <iostream>
#include <imgui.h>
#include "Game/MainMenu/MainMenu.hpp"
#include <cpplocate/cpplocate.h>

using std::runtime_error;
using std::string;

auto redengine::Engine::Run() -> void {
    auto &engine = redengine::Engine::get();
    engine.prefabRepo_.Init();

    //ResourceManager::getInstance().loadResources();
    engine.game_stack_.AddToStack(std::make_shared<MainMenu>());
    engine.game_stack_.getTop()->Init();

    engine.t_ = 0.0;
    engine.dt_ = 0.01;

    double current_time = glfwGetTime();
    double accumulator = 0.0;

    // State previous;
    // State current;
    // State state;
    // glfwFocusWindow(engine.window);
    engine.renderer_.Init();


    while (engine.GetIsRunning()) {
        double new_time = glfwGetTime();
        double frame_time = new_time - current_time;
        engine.engine_frame_time_ = frame_time;

        if (frame_time > 0.25)
            frame_time = 0.25;
        current_time = new_time;

        accumulator += frame_time;

        while (accumulator >= engine.dt_) {
            // previousState = currentState;
            glfwPollEvents();
            engine.ProcessInput(engine.dt_);
            engine.game_stack_.getTop()->FixedUpdate(engine.t_, engine.dt_);
            engine.t_ += engine.dt_;
            accumulator -= engine.dt_;
        }

        // const double alpha = accumulator / dt;
        // state = currentState * alpha + previousState * (1.0 - alpha);
        engine.game_stack_.getTop()->Update(engine.t_, engine.engine_frame_time_);
        engine.renderer_.Draw();
        if (engine.game_stack_.isRemoveTopFlag()) {
            engine.game_stack_.getTop()->UnInit();
        }
        engine.game_stack_.checkTop();
    }
    glfwDestroyWindow(engine.window_);
}

GUIManager &redengine::Engine::GetGuiManager() {
    return gui_manager_;
}

redengine::Engine::Engine() {
    SetupBasePath();
    log_.StartLog(GetBasePath().u8string());

    if (!glfwInit()) {
        std::cerr << "GLFW FAILED TO INIT \n";
    }
    //gleqInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    glsl_version_ = "#version 150";

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

    // glfw window creation
    // --------------------
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    last_window_x_size_ = mode->width / 2;
    last_window_y_size_ = mode->height / 2;
    window_ = glfwCreateWindow(last_window_x_size_, last_window_y_size_, "Project Blue", nullptr, nullptr);
    if (window_ == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    //gleqTrackWindow(window);
    glfwMakeContextCurrent(window_);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    input::GLFWInputWrangler::Init(window_);
    GUIManager::InitialiseImGUI(window_);
    // This allows us to use model 0 as an error model.
    // Are we industry pros yet?
    model_manager_.GetModelID(base_path_ / "res" / "model" / "error.fbx");
    log_.AddLog(ConsoleLog::LogType::Engine, "Engine Successfully Initialised", __LINE__, __FILE__);
}

redengine::Engine::~Engine() {
    glfwTerminate();
}

auto redengine::Engine::get() -> Engine & {
    static auto instance = Engine{};
    return instance;
}

auto redengine::Engine::ProcessInput(double deltaTime) -> void {
    using namespace input;
    GLFWEvent glfw_event;
    auto handledMouse = true;

    while (GLFWInputWrangler::PollEvent(glfw_event)) {
        auto event = input_manager_.ConvertEvent(glfw_event);
        if (event.type == InputType::kKeyPressed || event.type == input::InputType::kKeyReleased) {
            input_manager_.RecordKeyStates(event);
        }
        switch (event.type) {
            case InputType::kKeyPressed: {
                auto keyboard = std::get<InputEvent::KeyboardEvent>(event.data);
                if (keyboard.key == PhysicalKey::F1) {
                    auto mouseMode = glfwGetInputMode(window_, GLFW_CURSOR);
                    if (mouseMode == GLFW_CURSOR_NORMAL) {
                        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    } else {
                        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    }
                }
            }
                break;
            case InputType::kWindowClosed: {
                this->EndEngine();
            }
                break;
            default:
                break;
        }

        game_stack_.getTop()->HandleInputData(event, deltaTime);
    }
    if (!handledMouse) {
        this->mouse_ = {0.0f, 0.0f};
    }
}

bool redengine::Engine::GetMouseMode() {
    auto mouseMode = glfwGetInputMode(window_, GLFW_CURSOR);
    return mouseMode == GLFW_CURSOR_NORMAL;
}

void redengine::Engine::GetMouseMode(bool mode) {
    if (mode) {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

auto redengine::Engine::GetIsRunning() const -> bool {
    return this->is_running_;
}

auto redengine::Engine::EndEngine() -> void {
    is_running_ = false;
}

auto redengine::Engine::SetupBasePath() -> void {
    base_path_ = cpplocate::getExecutablePath();
    base_path_.remove_filename();
}

void redengine::Engine::SettingMenu() {
    ImVec2 buttonSize(150, 30);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    auto &mainWindow = redengine::Engine::get().window_;

    ImGui::SetNextWindowFocus();
    ImGui::SetNextWindowSize(ImVec2(500, 500), 1);
    ImGui::Begin("Settings", &show_settings_menu_,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::Text("Window Settings");
    if (ImGui::Button("Borderless Windowed", buttonSize)) {
        last_window_x_size_ = mode->width / 2;
        last_window_y_size_ = mode->height / 2;
        glfwSetWindowMonitor(mainWindow, nullptr, 0, 0, last_window_x_size_, last_window_y_size_, mode->refreshRate);
        this->renderer_.UpdateViewPort(0, 0, last_window_x_size_, last_window_y_size_);
        glfwSetWindowPos(mainWindow, last_window_x_size_ - last_window_x_size_ / 2,
                         last_window_y_size_ - last_window_y_size_ / 2);
    }
    ImGui::SameLine();
    if (ImGui::Button("Windowed", buttonSize)) {
        last_window_x_size_ = mode->width / 2;
        last_window_y_size_ = mode->height / 2;
        glfwSetWindowMonitor(window_, nullptr, 0, 0, last_window_x_size_, last_window_y_size_, mode->refreshRate);
        this->renderer_.UpdateViewPort(0, 0, last_window_x_size_, last_window_y_size_);
        glfwSetWindowPos(mainWindow, last_window_x_size_ - last_window_x_size_ / 2,
                         last_window_y_size_ - last_window_y_size_ / 2);
    }
    ImGui::SameLine();
    if (ImGui::Button("Borderless Fullscreen", buttonSize)) {
        glfwSetWindowMonitor(mainWindow, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
        this->renderer_.UpdateViewPort(0, 0, mode->width, mode->height);
    }

    if (ImGui::Button("Fullscreen", buttonSize)) {
        glfwSetWindowMonitor(mainWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        this->renderer_.UpdateViewPort(0, 0, mode->width, mode->height);
    }
    ImGui::Separator();
    ImGui::Text("Brightness");
    if (ImGui::SliderFloat("Gamma Correction", &gamma_correction_, 0.1f, 2.f,
                           "Gamma = %.1f")) {
        glfwSetGamma(monitor, gamma_correction_);
    }
    ImGui::End();
}

int redengine::Engine::GetLastWindowXSize() const {
    return last_window_x_size_;
}

void redengine::Engine::SetLastWindowXSize(int last_window_x_size) {
    Engine::last_window_x_size_ = last_window_x_size;
}

int redengine::Engine::GetLastWindowYSize() const {
    return last_window_y_size_;
}

void redengine::Engine::SetLastWindowYSize(int last_window_y_size) {
    Engine::last_window_y_size_ = last_window_y_size;
}

double redengine::Engine::GetT() const {
    return t_;
}

double redengine::Engine::GetDt() const {
    return dt_;
}

double redengine::Engine::GetFrameTime() const {
    return engine_frame_time_;
}

auto redengine::Engine::GetBasePath() const -> std::filesystem::path {
    return this->base_path_;
}


redengine::PrefabRepo &redengine::Engine::GetPrefabRepo() {
    return prefabRepo_;
}

ConsoleLog &redengine::Engine::GetLog() {
    return log_;
}
