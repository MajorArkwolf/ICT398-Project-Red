#include "PhysicsDemo.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Engine.hpp"
#include "Engine/SubModules/JsonLoader.hpp"
#include "Engine/Physics/PhysicsShape.hpp"

template<class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overload(Ts...) -> overload<Ts...>;

static inline void ToggleRenderer(physics::PhysicsWorld &pe, bool val) {
    if (pe.GetRendererStatus() != val) {
        pe.ToggleRenderer();
    }
}

PhysicsDemo::PhysicsDemo() {
    physics_world_.SetECS(&ecs_);
    camera = engine::Camera();
    camera.position_ = glm::vec3(0.0f, 10.0f, 0.0f);
    relativeMouse = true;
    std::filesystem::path path = "";
    path.append("PhysicsDemo");
    path.append("Scene.json");
    JSONLoader::LoadScene(path, &ecs_, &physics_world_);
}

void PhysicsDemo::Display(Shader *shader, const glm::mat4 &projection, const glm::mat4 &view) {
    auto &renderer = redengine::Engine::get().renderer_;
    auto &engine = redengine::Engine::get();
    auto &gui_manager = engine.GetGuiManager();
    ToggleRenderer(physics_world_, gui_manager.renderer_);
    renderer.SetCameraOnRender(camera);
    ecs_.Draw(shader, projection, view);
}

void PhysicsDemo::GUIStart() {
    auto &engine = redengine::Engine::get();
    GUIManager::startWindowFrame();
    engine.GetGuiManager().DisplayEscapeMenu();
    engine.GetGuiManager().DisplayConsoleLog();
}

void PhysicsDemo::GUIEnd() {
    GUIManager::EndWindowFrame();
}

void PhysicsDemo::Update(double t, double dt) {
    ecs_.Update(t, dt);
    camera.ProcessKeyboardInput(forward_, backward_, left_, right_, dt);
    //TODO: fix this to use just the phyiscs world instead.
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    physics_engine.Update(t, dt);
}

void PhysicsDemo::FixedUpdate(double t, double dt) {
    ecs_.FixedUpdate(t, dt);
    //TODO: fix this to use just the phyiscs world instead.
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    physics_engine.FixedUpdate(t, dt);
}

void PhysicsDemo::Init() {
}

void PhysicsDemo::UnInit() {
}

void PhysicsDemo::HandleInputData(input::InputEvent inputData, double deltaTime) {
    using namespace input;
    auto &engine = redengine::Engine::get();
    auto &gui_manager = engine.GetGuiManager();
    auto handledMouse = false;
    std::visit(overload{
                   [&](std::monostate) {

                   },
                   [&](InputEvent::MouseEvent mouse) {
                       switch (inputData.type) {
                           case input::InputType::kButtonPressed: {
                               switch (mouse.button) {
                                   case input::MouseButton::kLeft: {
                                       auto entity = ecs_.CreateEntity();

                                       entity.AddComponent<component::Model>(5);
                                       auto &trans = entity.AddComponent<component::Transform>();
                                       trans.pos = camera.position_;
                                       trans.rot = {1, 0, 0, 0};
                                       trans.scale = {1, 1, 1};

                                       auto &physbody = entity.AddComponent<component::PhysicBody>();
                                       physbody.SetVelocity(camera.front_);
                                       physics_world_.AddCollisionBody(entity.GetID(), {0, 0, 0}, {1, 0, 0, 0});
                                       static auto shape = engine.GetPhysicsEngine().CreateSphereShape(1.1);
                                       physics_world_.AddCollider(entity.GetID(), shape, {0, 0, 0}, {1, 0, 0, 0});


                                   } break;
                               }
                               break;
                           }
                       }
                   },
                   [&](InputEvent::KeyboardEvent keyboard) {
                       switch (inputData.type) {
                           case input::InputType::kKeyPressed: {
                               switch (keyboard.key) {
                                   case input::VirtualKey::W: {
                                       forward_ = true;
                                   } break;
                                   case input::VirtualKey::A: {
                                       left_ = true;
                                   } break;
                                   case input::VirtualKey::S: {
                                       backward_ = true;
                                   } break;
                                   case input::VirtualKey::D: {
                                       right_ = true;
                                   } break;
                                   case input::VirtualKey::kEscape: {

                                   } break;
                               }
                           } break;
                           case input::InputType::kKeyReleased: {
                               switch (keyboard.key) {
                                   case input::VirtualKey::W: {
                                       forward_ = false;
                                   } break;
                                   case input::VirtualKey::A: {
                                       left_ = false;
                                   } break;
                                   case input::VirtualKey::S: {
                                       backward_ = false;
                                   } break;
                                   case input::VirtualKey::D: {
                                       right_ = false;
                                   } break;
                                   case input::VirtualKey::kEscape: {
                                       gui_manager.ToggleWindow("escapeMenu");
                                   } break;
                               }
                           } break;
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
                           } break;
                           default:
                               break;
                       }
                   }},
               inputData.data);
    if (!handledMouse) {
        engine.mouse_ = {0.0f, 0.0f};
    }
}
