#include "Demo.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Engine.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

static inline void ToggleRenderer(physics::PhysicsEngine& pe, bool val) {
    if (pe.GetRendererStatus() != val) {
        pe.ToggleRenderer();
        std::cout << "toggle\n";
    }
}

template<class... Ts>
struct overload : Ts ... {
    using Ts::operator()...;
};
template<class... Ts> overload(Ts...)->overload<Ts...>;

Demo::Demo() {
    camera = engine::Camera();
    camera.position_ = glm::vec3(0.0f, 10.0f, 0.0f);
    relativeMouse = true;
    physics_engine_.SetECS(&ecs_);
    std::filesystem::path path = "";
    path.append("Demo");
    path.append("Scene.json");
    JSONLoader::LoadScene(path, &ecs_, &physics_engine_);
}

void Demo::Init() {
}

void Demo::UnInit() {
}

void Demo::Display(Shader *shader, const glm::mat4 &projection, const glm::mat4 &view) {
    auto &renderer = redengine::Engine::get().renderer_;
    auto &engine = redengine::Engine::get();
    auto &gui_manager = engine.GetGuiManager();
    renderer.SetCameraOnRender(camera);
    ToggleRenderer(physics_engine_, gui_manager.renderer_);
    ecs_.Draw(shader, projection, view);
    physics_engine_.Draw(projection, view);
}

void Demo::GUIStart() {
    auto &engine = redengine::Engine::get();
    GUIManager::startWindowFrame();
    engine.GetGuiManager().DisplayEscapeMenu();
    engine.GetGuiManager().DisplayConsoleLog();
    engine.GetGuiManager().DisplayDevScreen(camera);
    engine.GetGuiManager().DisplayInputRebindWindow();

}

void Demo::GUIEnd() {
    GUIManager::EndWindowFrame();
}

void Demo::Update(double t, double dt) {
    ecs_.Update(t, dt);
    camera.ProcessKeyboardInput(forward_, backward_, left_, right_, dt);
    physics_engine_.Update(t, dt);
}

void Demo::FixedUpdate(double t, double dt) {
    ecs_.FixedUpdate(t, dt);
    physics_engine_.FixedUpdate(t, dt);
}

void Demo::HandleInputData(input::InputEvent inputData, double deltaTime) {
    using namespace input;
    auto &engine = redengine::Engine::get();
    auto &gui_manager = engine.GetGuiManager();
    auto handledMouse = false;
    std::visit(overload{
            [&](std::monostate) {

            }, [&](InputEvent::MouseEvent mouse) {

            },
            [&](InputEvent::KeyboardEvent keyboard) {

                switch (inputData.type) {
                    case input::InputType::kKeyPressed: {
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
                    case input::InputType::kKeyReleased: {
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
                            case input::VirtualKey::kEscape:
                                gui_manager.ToggleWindow("escapeMenu");
                        } break;
                        default:
                            break;
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
        engine.mouse_ = {0.0f, 0.0f};
    }
}
