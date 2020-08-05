#include "ECS/Entity.hpp"
#include "Demo.hpp"
#include "Engine/Engine.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Basic.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>;
// TEST
#include "Engine/Engine.hpp"

Demo::Demo() {
    camera = Engine::Camera();
    camera.Position = glm::vec3(0.0f, 10.0f, 0.0f);
    relativeMouse = true;
    auto entity = JSONLoader::LoadEntity("./res/Entity/Test.json", this->ecs);
    auto entity2 = JSONLoader::LoadEntity("./res/Entity/Test2.json", this->ecs);
}

void Demo::Init() {

}

void Demo::UnInit() {

}

void Demo::Display(const glm::mat4& projection, const glm::mat4& view) {
    auto &renderer = RedEngine::Engine::get().renderer;
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
    ecs.FixedUpdate(t, dt);
}

void Demo::HandleInputData(input::InputEvent inputData, double deltaTime) {
    using namespace input;
    auto &engine      = RedEngine::Engine::get();
    auto &guiManager  = engine.getGuiManager();
    auto handledMouse = false;
    std::visit(overload{
            [&](std::monostate)
            {

            },[&](InputEvent::MouseEvent mouse)
            {

            },
            [&](InputEvent::KeyboardEvent keyboard)
            {

            },
            [&](InputEvent::dVector2 vec)
            {

            },
            [&](InputEvent::iVector2 vec)
            {

                switch(inputData.type)
                {
                    case input::InputType::kCursorMoved:
                    {
                        static int prev_x = 0;
                        static int prev_y = 0;

                        auto x = static_cast<double>(vec.x);
                        auto y = static_cast<double>(vec.y);
                        x     = x * -1.0;
                        camera.ProcessMouseMovement(prev_x - x, prev_y - y);
                        handledMouse = true;
                        prev_x = x;
                        prev_y = y;
                    }break;
                    default:break;
                }
            }
    }, inputData.data);

//    switch (inputData.type) {
//        case BLUE_InputType::KEY_PRESS: { //  Key Press events
//
//            switch (inputData.inputAction) {
//                case BLUE_InputAction::INPUT_MOVE_FORWARD: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_BACKWARD: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_LEFT: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_RIGHT: {
//                } break;
//                default: break;
//            }
//        } break;
//        case BLUE_InputType::KEY_RELEASE: { // Key Release events
//            switch (inputData.inputAction) {
//                case BLUE_InputAction::INPUT_MOVE_FORWARD: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_BACKWARD: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_LEFT: {
//                } break;
//                case BLUE_InputAction::INPUT_MOVE_RIGHT: {
//                } break;
//                case BLUE_InputAction::INPUT_JUMP: {
//                } break;
//                case BLUE_InputAction::INPUT_CROUCH: {
//                } break;
//                case BLUE_InputAction::INPUT_ACTION_2: {
//                } break;
//                case BLUE_InputAction::INPUT_ACTION_3: {
//                } break;
//                case BLUE_InputAction::INPUT_ACTION_4: {
//                } break;
//                default: break;
//            }
//        } break;
//        case BLUE_InputType::MOUSE_MOTION: { // Mouse motion event
//            if (!engine.getMouseMode()) {
////                 auto x = static_cast<double>(inputData.mouseMotionRelative.x);
////                 auto y = static_cast<double>(inputData.mouseMotionRelative.y);
////                 y      = y * -1.0;
////                 camera.ProcessMouseMovement(x, y);
////                 handledMouse = true;
//            }
//        } break;
//        case BLUE_InputType::MOUSE_WHEEL: { // Mouse Wheel event
//            auto amountScrolledY = static_cast<double>(inputData.mouseWheelMotion);
//            //camera.ProcessMouseScroll(amountScrolledY);
//        } break;
//        case BLUE_InputType::WINDOW_RESIZE: {
//            this->handleWindowEvent();
//        } break;
//        default: break;
//    }
    if (!handledMouse) {
        engine.mouse = {0.0f, 0.0f};
    }
}
