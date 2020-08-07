#include "Demo.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Engine.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

template<class... Ts> struct overload : Ts ... {	using Ts::operator()...;};
template<class... Ts> overload(Ts...)->overload<Ts...>;
// TEST
#include "Engine/Engine.hpp"

Demo::Demo() {
	camera = engine::Camera();
	camera.position_ = glm::vec3(0.0f, 10.0f, 0.0f);
	relativeMouse = true;
	auto entity = JSONLoader::LoadEntity("./res/Entity/Test.json", this->ecs_);
	auto entity2 = JSONLoader::LoadEntity("./res/Entity/Test2.json", this->ecs_);
}

void Demo::Init() {
}

void Demo::UnInit() {
}

void Demo::Display(const glm::mat4& projection, const glm::mat4& view) {
	auto& renderer = RedEngine::Engine::get().renderer;
	renderer.SetCameraOnRender(camera);
	ecs.Draw(projection, view, camera.getLocation());
}

void Demo::GUIStart() {
}

void Demo::GUIEnd() {
}

void Demo::Update(double t, double dt) {
	ecs.Update(t, dt);
}

void Demo::FixedUpdate(double t, double dt) {
	ecs_.FixedUpdate(t, dt);
}

void Demo::HandleInputData(input::InputEvent inputData, double deltaTime) {
	using namespace input;
	auto& engine = RedEngine::Engine::get();
	auto& guiManager = engine.getGuiManager();
	auto handledMouse = false;
	std::visit(overload{
			[&](std::monostate) {

			}, [&](InputEvent::MouseEvent mouse) {

			},
			[&](InputEvent::KeyboardEvent keyboard) {

				switch (inputData.type) {
					case input::InputType::kKeyPressed:
						{
							switch (keyboard.key) {
								case input::VirtualKey::W: {
									forward_ = true;
								}
									break;
								case input::VirtualKey::A: {
									left_ = true;
								}
									break;
								case input::VirtualKey::S: {
									backward_ = true;
								}
									break;
								case input::VirtualKey::D: {
									right_ = true;
								}
									break;
							}
						}
						break;
					case input::InputType::kKeyReleased:
						{
							switch (keyboard.key) {
								case input::VirtualKey::W: {
									forward_ = false;
								}
									break;
								case input::VirtualKey::A: {
									left_ = false;
								}
									break;
								case input::VirtualKey::S: {
									backward_ = false;
								}
									break;
								case input::VirtualKey::D: {
									right_ = false;
								}
									break;
							}
						}
						break;
				}

			},
			[&](InputEvent::dVector2 vec) {

			},
			[&](InputEvent::iVector2 vec) {

				switch (inputData.type) {
					case input::InputType::kCursorMoved: {
						static int prev_x = 0;
						static int prev_y = 0;

						auto x = static_cast<double>(vec.x);
						auto y = static_cast<double>(vec.y);
						x = x * -1.0;
						camera.ProcessMouseMovement(prev_x - x, prev_y - y);
						handledMouse = true;
						prev_x = x;
						prev_y = y;
					}
						break;
					default:
						break;
				}
			}
		}, inputData.data);
	if (!handledMouse) {
		engine.mouse = { 0.0f, 0.0f };
	}
}
