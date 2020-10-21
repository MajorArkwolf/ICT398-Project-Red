#include "PhysicsDemo.hpp"

#include "DataStructures/Model/Overload.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Physics/PhysicsShape.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

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
    auto &renderer = redengine::Engine::get().renderer_;
    renderer.SetCameraOnRender(camera);
    physics_world_.SetECS(&ecs_);
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
                                       auto &entity = ecs_.CreateEntity();

                                       auto &model = entity.AddComponent<component::Model>(5);

                                       auto &trans = entity.AddComponent<component::Transform>();
                                       trans.pos = camera.position_;
                                       trans.rot = {1, 0, 0, 0};
                                       trans.scale = glm::vec3{20, 20, 20};
                                       ;
                                       constexpr float radius = 1.1f;
                                       auto &physbody = entity.AddComponent<component::PhysicBody>();
                                       physbody.AddForce(camera.front_ * 50.0f);
                                       physbody.AddTorque({0.f, 0, 0});
                                       physbody.mass = 0.1;
                                       physbody.inverse_mass = 1 / 0.1;
                                       physbody.inertia_tensor = glm::mat3((2.f / 5.f) * physbody.mass * (radius * radius));
                                       physbody.inverse_inertia_tensor = glm::inverse(physbody.inertia_tensor);
                                       physics_world_.AddCollisionBody(entity.GetID(), {0, 0, 0}, {1, 0, 0, 0});
                                       static auto shape = engine.GetPhysicsEngine().CreateSphereShape(radius);
                                       physics_world_.AddCollider(entity.GetID(), shape, {0, 0, 0}, {1, 0, 0, 0});

                                   } break;
                                   case input::MouseButton::kRight: {
                                       auto entity = ecs_.CreateEntity();

                                       entity.AddComponent<component::Model>(5);
                                       auto &trans = entity.AddComponent<component::Transform>();
                                       trans.pos = camera.position_;
                                       trans.rot = {1, 0, 0, 0};
                                       trans.scale = glm::vec3{2.0f, 2.0f, 2.0f};
                                       constexpr float radius = 1.1f;
                                       auto &physbody = entity.AddComponent<component::PhysicBody>();
                                       physbody.AddForce({0, 0, 0});
                                       physbody.AddTorque({0.f, 500, 0});
                                       physbody.mass = 500;
                                       physbody.inverse_mass = 1 / physbody.mass;
                                       physbody.inertia_tensor = glm::mat3((2.f / 5.f) * physbody.mass * (radius * radius));
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
                                   case input::VirtualKey::M: {
                                       physics_world_.SetGravity(glm::vec3(0, -9.8f, 0));
                                       physics_world_.SetGravityEnabled(true);
                                   } break;
                                   case input::VirtualKey::V: {
                                       auto entity = ecs_.CreateEntity();
                                       constexpr redengine::Box box_shape = {glm::vec3{3, 5, 3}};
                                       entity.AddComponent<component::Model>(1);
                                       auto &trans = entity.AddComponent<component::Transform>();
                                       trans.pos = camera.position_;
                                       trans.rot = {1, 0, 0, 0};
                                       trans.scale = glm::vec3{1.0f, 1.0f, 1.0f};
                                       auto &physbody = entity.AddComponent<component::PhysicBody>();
 
                                       physbody.mass = 20;
                                       physbody.inverse_mass = 1 / 20;

                                       physics_world_.AddCollisionBody(entity.GetID(), {0, 0, 0}, {1, 0, 0, 0});
                                       static auto shape = engine.GetPhysicsEngine().CreateBoxShape({box_shape.extents.x, box_shape.extents.y, box_shape.extents.z});
                                       physbody.inertia_tensor = engine.GetPhysicsEngine().CalculateInertiaTensor(box_shape, 20);
                                       physbody.inverse_inertia_tensor = glm::inverse(physbody.inertia_tensor);
                                       physics_world_.AddCollider(entity.GetID(), shape, {0, 0, 0}, {1, 0, 0, 0});
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
                                   case input::VirtualKey::k1: {
                                       ecs_ = {};
                                       physics_world_.ResetWorld();
                                       physics_world_.SetECS(&ecs_);
                                       std::filesystem::path path;
                                       path = path / "PhysicsDemo" / "Scene1" / "Scene.json";
                                       JSONLoader::LoadScene(path, &ecs_, &physics_world_);
                                       redengine::Engine::get().GetMouseMode(relativeMouse);
                                   } break;
                                   case input::VirtualKey::k2: {
                                       ecs_ = {};
                                       physics_world_.ResetWorld();
                                       physics_world_.SetECS(&ecs_);
                                       std::filesystem::path path;
                                       path = path / "PhysicsDemo" / "Scene2" / "Scene.json";
                                       JSONLoader::LoadScene(path, &ecs_, &physics_world_);
                                       redengine::Engine::get().GetMouseMode(relativeMouse);
                                   } break;
                                   case input::VirtualKey::k3: {
                                       ecs_ = {};
                                       physics_world_.ResetWorld();
                                       physics_world_.SetECS(&ecs_);
                                       std::filesystem::path path;
                                       path = path / "PhysicsDemo" / "Scene3" / "Scene.json";
                                       JSONLoader::LoadScene(path, &ecs_, &physics_world_);
                                       redengine::Engine::get().GetMouseMode(relativeMouse);
                                   } break;
                                   case input::VirtualKey::k4: {
                                       ecs_ = {};
                                       physics_world_.ResetWorld();
                                       physics_world_.SetECS(&ecs_);
                                       std::filesystem::path path;
                                       path = path / "PhysicsDemo" / "Scene4" / "Scene.json";
                                       JSONLoader::LoadScene(path, &ecs_, &physics_world_);
                                       redengine::Engine::get().GetMouseMode(relativeMouse);
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
