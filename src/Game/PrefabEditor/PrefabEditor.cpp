#include "PrefabEditor.hpp"

#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Engine.hpp"
#include "Engine/SubModules/JsonLoader.hpp"

#define M_PI 3.1415926535f

template<class... Ts>
struct overload : Ts ... {
    using Ts::operator()...;
};
template<class... Ts> overload(Ts...)->overload<Ts...>;

PrefabEditor::PrefabEditor() {
    physics_world_.SetECS(&ecs_);
    camera = engine::Camera();
    camera.position_ = glm::vec3(0.0f, 10.0f, 0.0f);
    relativeMouse = true;
//    std::filesystem::path path = "";
//    path.append("PrefabEditor");
//    path.append("Scene.json");
//    JSONLoader::LoadScene(path, &ecs_, &physics_world_);
}

void PrefabEditor::Display(Shader *shader, const glm::mat4 &projection, const glm::mat4 &view) {
    auto &renderer = redengine::Engine::get().renderer_;
    renderer.SetCameraOnRender(camera);
    ecs_.Draw(shader, projection, view);
    prefab_gui_.Draw(shader, projection, view);
}

void PrefabEditor::GUIStart() {
    auto &engine = redengine::Engine::get();
    GUIManager::startWindowFrame();
    engine.GetGuiManager().DisplayEscapeMenu();
    engine.GetGuiManager().DisplayConsoleLog();
}

void PrefabEditor::GUIEnd() {
    GUIManager::EndWindowFrame();
}

void PrefabEditor::Update(double t, double dt) {
    camera.ProcessKeyboardInput(forward_, backward_, left_, right_, dt);
    ecs_.Update(t, dt);
}

void PrefabEditor::FixedUpdate(double t, double dt) {
    ecs_.FixedUpdate(t, dt);
}

void PrefabEditor::Init() {

}

void PrefabEditor::UnInit() {

}

void PrefabEditor::HandleInputData(input::InputEvent inputData, double deltaTime) {
    using namespace input;
    auto &engine = redengine::Engine::get();
    auto &gui_manager = engine.GetGuiManager();
    auto handledMouse = false;
    std::visit(overload{
            [&](std::monostate) {

            }, [&](InputEvent::MouseEvent mouse) {
                switch (inputData.type) {
                    case (input::InputType::kButtonPressed): {
                        switch (mouse.button) {
                            case input::MouseButton::kRight: {
                                camera_enabled_ = true;
                            }
                                break;
                            default:
                                break;
                        }
                    }
                        break;
                    case (input::InputType::kButtonReleased): {
                        switch (mouse.button) {
                            case input::MouseButton::kRight: {
                                camera_enabled_ = false;
                            }
                                break;
                            default:
                                break;
                        }
                    }
                    default:
                        break;
                }
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
                            case input::VirtualKey::kEscape: {

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
                            case input::VirtualKey::kEscape: {
                                gui_manager.ToggleWindow("escapeMenu");
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
                        //camera.ProcessMouseMovement(prev_x - x, prev_y - y);
                        ArcBallCamera(prev_x - x, prev_y - y);
                        handledMouse = true;
                        prev_x = x;
                        prev_y = y;
                    }
                        break;
                    case input::InputType::kMouseScrolled: {
                        double amountScrolledY = static_cast<double>(vec.y);
                        distanceFromEntity += amountScrolledY;
                        if (distanceFromEntity < 0) {
                            distanceFromEntity = 0;
                        }
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

void PrefabEditor::ArcBallCamera(double dx, double dy) {
    if (camera_enabled_) {
        auto viewport = redengine::Engine::get().renderer_.GetViewPort();

        auto rotation = currentRotation * ((std::atan(1) * 4) / 180);
        camera.position_.x = std::cos(rotation) * distanceFromEntity;
        camera.position_.z = std::sin(rotation) * distanceFromEntity;
        camera.position_.y += dy;
        currentRotation = fmod(currentRotation + dx, 360);
        camera.front_ = -camera.position_;
        //// Get the homogenous position of the camera and pivot point
//        glm::vec4 position = {camera.position_, 1.0f};
//        glm::vec4 pivot = {(camera.position_ + camera.front_), 1.0f};
//        //glm::vec4 position(app->m_camera.GetEye().x, app->m_camera.GetEye().y, app->m_camera.GetEye().z, 1);
//        //glm::vec4 pivot(app->m_camera.GetLookAt().x, app->m_camera.GetLookAt().y, app->m_camera.GetLookAt().z, 1);
//
//// step 1 : Calculate the amount of rotation given the mouse movement.
//        float deltaAngleX = (2 * M_PI / viewport.x); // a movement from left to right = 2*PI = 360 deg
//        float deltaAngleY = (M_PI / viewport.y);  // a movement from top to bottom = PI = 180 deg
//        float xAngle = static_cast<float>(dx) * deltaAngleX;
//        float yAngle = static_cast<float>(dx) * deltaAngleY;
//
//// Extra step to handle the problem when the camera direction is the same as the up vector
//        float cosAngle = dot(camera.front_, camera.up_);
//        //float cosAngle = dot(app->m_camera.GetViewDir(), app->m_upVector);
//        //TODO: Reimplement this, What is yDeltaAngle? http://asliceofrendering.com/camera/2019/11/30/ArcballCamera/
//        if (cosAngle * sgn(yDeltaAngle) > 0.99f)
//            yAngle = 0;
//
//// step 2: Rotate the camera around the pivot point on the first axis.
//        glm::mat4x4 rotationMatrixX(1.0f);
//        rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, camera.up_); // COULD BE WRONG
//        position = (rotationMatrixX * (position - pivot)) + pivot;
//
//// step 3: Rotate the camera around the pivot point on the second axis.
//        glm::mat4x4 rotationMatrixY(1.0f);
//        rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, camera.GetRightVector()); // COULD BE WRONG
//        glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;
//
//        // Update the camera view (we keep the same lookat and the same up vector)
//        camera.SetCameraView(finalPosition, camera.position_, camera.up_);
//        //app->m_camera.SetCameraView(finalPosition, app->m_camera.GetLookAt(), app->m_upVector);
    }
}