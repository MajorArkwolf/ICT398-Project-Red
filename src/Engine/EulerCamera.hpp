#pragma once

#include <vector>

#include <glad/glad.h>  // NOLINT
#include <glm/glm.hpp>

/// Makes understanding the direction simple.
enum class CameraMovement {
    kForward,
    kBackward,
    kLeft,
    kRight
};

/// Default values used to initialise a camera.
constexpr double kYaw = 0.0;
constexpr double kPitch = 0.0;
constexpr double kSpeed = 0.05;
constexpr double kSensitivity = 0.1;
constexpr double kZoom = 45.0;

namespace engine {

/**
 * Euler Camera designed to interact in a 3D environment.
 */
class Camera {
 public:
  /// Camera Attributes
  glm::vec3 position_ = {};
  glm::vec3 front_ = {};
  glm::vec3 up_ = {};
  glm::vec3 right_ = {};
  glm::vec3 world_up_ = {};

  /// Euler Angles
  double yaw_ = {};
  double pitch_ = {};

  /// Camera options
  float movement_speed_ = {};
  double mouse_sensitivity_ = {};
  double zoom_ = {};

  /**
   * Default Constructor
   * @param position the position of the camera.
   * @param up The up axis
   * @param yaw of the camera.
   * @param pitch of the camera.
   */
  explicit Camera(glm::dvec3 position = glm::dvec3(0.0f, 0.0f, 0.0f),
                  glm::dvec3 up = glm::dvec3(0.0f, 1.0f, 0.0f),
                  double yaw = kYaw,
                  double pitch = kPitch);

  /**
   * Overloaded constructor
   * @param pos_x the position on the x axis.
   * @param pos_y the position on the y axis.
   * @param pos_z the position on the z axis.
   * @param up_x position on the x axis.
   * @param up_y position on the y axis.
   * @param up_z position on the z axis.
   * @param yaw of the camera.
   * @param pitch of the camera.
   */
  Camera(double pos_x, double pos_y, double pos_z, double up_x, double up_y,
         double up_z, double yaw, double pitch);

  /**
   * Calculates the view matrix used for shaders and the rendering engine.
   * @return Gets the view matrix
   */
  glm::mat4 GetViewMatrix();

  /**
   * Applies the movement based on the default values and delta time.
   * @param direction input received from any keyboard-like input system
   * @param delta_time produced by the engine.
   */
  void ProcessKeyboard(CameraMovement direction, double delta_time);

  /**
   * Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
   * @param x_offset the mouses x offset.
   * @param y_offset the mouse y offset
   * @param constrain_pitch ensures the camera cant be flipped.
   */
  void ProcessMouseMovement(double x_offset, double y_offset,
                            GLboolean constrain_pitch = true);

  /**
   * Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
   * @param y_offset the offset of the mouse.
   */
  void ProcessMouseScroll(double y_offset);

  /**
   * Gets the location of the camera on the x and y axis.
   * @return the location of the camera
   */
  glm::dvec2 GetLocation() const;

  /**
   * The right vector of the camera
   * @return right vector
   */
  glm::vec3 GetRightVector();
  glm::vec3 GetFrontVector();

  void ProcessKeyboardInput(bool forward, bool backward, bool left,
                            bool right, double delta_time);
  /**
   * Calculates the front vector from the Camera's (updated) Euler Angles
   */
  void UpdateCameraVectors();
};

}  // namespace engine
