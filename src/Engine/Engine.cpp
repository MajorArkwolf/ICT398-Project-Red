#include "Engine/Engine.hpp"

#include <iostream>
#include <imgui.h>
// Game States
#include "Game/MainMenu/MainMenu.hpp"
#include <cpplocate/cpplocate.h>
#include "Engine/SubModules/Input/GLFWInputWrangler.hpp"


using std::runtime_error;
using std::string;

auto RedEngine::Engine::run() -> void {
	auto& engine = RedEngine::Engine::get();

	//ResourceManager::getInstance().loadResources();
	engine.gameStack.AddToStack(std::make_shared<MainMenu>());
	engine.gameStack.getTop()->Init();

	engine.t = 0.0;
	engine.dt = 0.01;

	double currentTime = glfwGetTime();
	double accumulator = 0.0;

	// State previous;
	// State current;
	// State state;
	glfwFocusWindow(engine.window);
	engine.renderer.Init();


	while (engine.getIsRunning()) {
		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;
		engine.EngineFrameTime = frameTime;

		if (frameTime > 0.25)
			frameTime = 0.25;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= engine.dt) {
			// previousState = currentState;
			glfwPollEvents();
			engine.processInput(engine.dt);
			engine.gameStack.getTop()->FixedUpdate(engine.t, engine.dt);
			engine.t += engine.dt;
			accumulator -= engine.dt;
		}

		// const double alpha = accumulator / dt;
		// state = currentState * alpha + previousState * (1.0 - alpha);
		engine.gameStack.getTop()->Update(engine.t, engine.EngineFrameTime);
		engine.renderer.Draw();
		if (engine.gameStack.isRemoveTopFlag()) {
			engine.gameStack.getTop()->UnInit();
		}
		engine.gameStack.checkTop();
	}
	glfwDestroyWindow(engine.window);
}

GUIManager& RedEngine::Engine::getGuiManager() {
	return guiManager;
}

RedEngine::Engine::Engine() {
	setBasePath();
	if (!glfwInit()) {
		std::cerr << "GLFW FAILED TO INIT \n";
	}
	//gleqInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
	glsl_version = "#version 150";

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
		GL_TRUE);
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

	// glfw window creation
	// --------------------
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	lastWindowXSize = mode->width / 2;
	lastWindowYSize = mode->height / 2;
	window = glfwCreateWindow(lastWindowXSize, lastWindowYSize, "Project Blue", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	//gleqTrackWindow(window);
	glfwMakeContextCurrent(window);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	input::GLFWInputWrangler::TrackWindow(window);

	GUIManager::initialiseImGUI(window);
	// This allows us to use model 0 as an error model.
	// Are we industry pros yet?
	modelManager.GetModelID("res/model/error.fbx");
}

RedEngine::Engine::~Engine() {
	glfwTerminate();
}

auto RedEngine::Engine::get() -> Engine& {
	static auto instance = Engine{};
	return instance;
}

auto RedEngine::Engine::processInput(double deltaTime) -> void {
	using namespace input;
	GLFWEvent glfwEvent;
	auto handled_Mouse = true;
	auto& input_manager = GetInputManager();

	while (GLFWInputWrangler::PollEvent(glfwEvent)) {
		auto event = input_manager.ConvertEvent(glfwEvent);
		if (event.type == InputType::kKeyPressed || event.type == InputType::kKeyReleased) {
			input_manager.RecordKeyStates(event);
		}
		switch (event.type) {
		case InputType::kKeyPressed: {
			auto keyboard = std::get<InputEvent::KeyboardEvent>(event.data);
			if (keyboard.key == VirtualKey::F1)
			{
				auto mouseMode = glfwGetInputMode(window, GLFW_CURSOR);
				if (mouseMode == GLFW_CURSOR_NORMAL) {
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
				else {
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
			}
		} break;
		case InputType::kWindowClosed: {
			this->endEngine();
		} break;
		default: break;
		}

		gameStack.getTop()->HandleInputData(event, dt);
	}
	if (!handled_Mouse) {
		this->mouse = { 0.0f, 0.0f };
	}
}

bool RedEngine::Engine::getMouseMode() {
	auto mouseMode = glfwGetInputMode(window, GLFW_CURSOR);
	return mouseMode == GLFW_CURSOR_NORMAL;
}

void RedEngine::Engine::setMouseMode(bool mode) {
	if (mode) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

auto RedEngine::Engine::getIsRunning() const -> bool {
	return this->isRunning;
}

auto RedEngine::Engine::endEngine() -> void {
	isRunning = false;
}

auto RedEngine::Engine::setBasePath() -> void {
	basepath = cpplocate::getExecutablePath();
	basepath.remove_filename();
}

void RedEngine::Engine::SettingMenu() {
	ImVec2 buttonSize(150, 30);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	auto& mainWindow = RedEngine::Engine::get().window;

	ImGui::SetNextWindowFocus();
	ImGui::SetNextWindowSize(ImVec2(500, 500), 1);
	ImGui::Begin("Settings", &showSettingsMenu,
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	ImGui::Text("Window Settings");
	if (ImGui::Button("Borderless Windowed", buttonSize)) {
		lastWindowXSize = mode->width / 2;
		lastWindowYSize = mode->height / 2;
		glfwSetWindowMonitor(mainWindow, nullptr, 0, 0, lastWindowXSize, lastWindowYSize, mode->refreshRate);
		this->renderer.UpdateViewPort(0, 0, lastWindowXSize, lastWindowYSize);
		glfwSetWindowPos(mainWindow, lastWindowXSize - lastWindowXSize / 2, lastWindowYSize - lastWindowYSize / 2);
	}
	ImGui::SameLine();
	if (ImGui::Button("Windowed", buttonSize)) {
		lastWindowXSize = mode->width / 2;
		lastWindowYSize = mode->height / 2;
		glfwSetWindowMonitor(window, nullptr, 0, 0, lastWindowXSize, lastWindowYSize, mode->refreshRate);
		this->renderer.UpdateViewPort(0, 0, lastWindowXSize, lastWindowYSize);
		glfwSetWindowPos(mainWindow, lastWindowXSize - lastWindowXSize / 2, lastWindowYSize - lastWindowYSize / 2);
	}
	ImGui::SameLine();
	if (ImGui::Button("Borderless Fullscreen", buttonSize)) {
		glfwSetWindowMonitor(mainWindow, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
		this->renderer.UpdateViewPort(0, 0, mode->width, mode->height);
	}

	if (ImGui::Button("Fullscreen", buttonSize)) {
		glfwSetWindowMonitor(mainWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		this->renderer.UpdateViewPort(0, 0, mode->width, mode->height);
	}
	ImGui::Separator();
	ImGui::Text("Brightness");
	if (ImGui::SliderFloat("Gamma Correction", &gammaCorrection, 0.1f, 2.f,
		"Gamma = %.1f")) {
		//SDL_SetWindowBrightness(this->window.get(), gammaCorrection);
		glfwSetGamma(monitor, gammaCorrection);
	}
	ImGui::End();
}

int RedEngine::Engine::getLastWindowXSize() const {
	return lastWindowXSize;
}

void RedEngine::Engine::setLastWindowXSize(int newLastWindowXSize) {
	Engine::lastWindowXSize = newLastWindowXSize;
}

int RedEngine::Engine::getLastWindowYSize() const {
	return lastWindowYSize;
}

void RedEngine::Engine::setLastWindowYSize(int newLastWindowYSize) {
	Engine::lastWindowYSize = newLastWindowYSize;
}
double RedEngine::Engine::getT() const {
	return t;
}
double RedEngine::Engine::getDt() const {
	return dt;
}

double RedEngine::Engine::getFrameTime() const {
	return EngineFrameTime;
}

auto RedEngine::Engine::getBasePath() const -> std::filesystem::path {
	return this->basepath;
}

input::RedEventManager& RedEngine::Engine::GetEventManager()
{
	return EventManager;
}

input::InputManager& RedEngine::Engine::GetInputManager()
{
	return InputManager;
}
