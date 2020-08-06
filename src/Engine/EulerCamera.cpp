#include "EulerCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

// Constructor with vectors
Camera::Camera(glm::dvec3 position, glm::dvec3 up, double yaw, double pitch)
    : front_(glm::dvec3(0.0f, 0.0f, -1.0f)), movement_speed_(kSpeed),
      mouse_sensitivity_(kSensitivity), zoom_(kZoom) {
  position_ = position;
  world_up_ = up;
  yaw_ = yaw;
  pitch_ = pitch;
  UpdateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(double pos_x, double pos_y, double pos_z, double up_x,
               double up_y, double up_z, double yaw, double pitch)
    : front_(glm::dvec3(0.0f, 0.0f, -1.0f)), movement_speed_(kSpeed),
      mouse_sensitivity_(kSensitivity), zoom_(kZoom) {
  position_ = glm::dvec3(pos_x, pos_y, pos_z);
  world_up_ = glm::dvec3(up_x, up_y, up_z);
  yaw_ = yaw;
  pitch_ = pitch;
  UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::ProcessKeyboard(CameraMovement direction, double delta_time) {
  const auto velocity =
      static_cast<float>(movement_speed_ * delta_time * 1000.0);
  if (direction == CameraMovement::kForward)
    position_ += front_ * velocity;
  if (direction == CameraMovement::kBackward)
    position_ -= front_ * velocity;
  if (direction == CameraMovement::kLeft)
    position_ -= right_ * velocity;
  if (direction == CameraMovement::kRight)
    position_ += right_ * velocity;
}

void Camera::ProcessMouseMovement(double x_offset, double y_offset,
                                  GLboolean constrain_pitch) {
  x_offset *= mouse_sensitivity_;
  y_offset *= mouse_sensitivity_;

  yaw_ += x_offset;
  pitch_ += y_offset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrain_pitch) {
    if (pitch_ > 89.0)
      pitch_ = 89.0;
    if (pitch_ < -89.0)
      pitch_ = -89.0;
  }

  // Update front_, right_ and up_ Vectors using the updated Euler angles
  UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(double y_offset) {
  if (zoom_ >= 1.0 && zoom_ <= 45.0)
    zoom_ -= y_offset;
  if (zoom_ <= 1.0)
    zoom_ = 1.0;
  if (zoom_ >= 45.0)
    zoom_ = 45.0;
}

void Camera::UpdateCameraVectors() {
  // Calculate the new front_ vector
  glm::dvec3 front;
  front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front.y = sin(glm::radians(pitch_));
  front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_ = glm::normalize(front);
  // Also re-calculate the right_ and up_ vector
  right_ =
      glm::normalize(glm::cross(front_, world_up_));
  // Normalize the vectors, because their length gets
  // closer to 0 the more you look up or down which results in slower movement.
  up_ = glm::normalize(glm::cross(right_, front_));
}

glm::dvec2 Camera::GetLocation() const {
  auto key = glm::dvec2(position_.x, position_.z);
  return key;
}

glm::vec3 Camera::GetRightVector() {
  // Normalize the vectors, because their length gets
  // closer to 0 the more you look up or down which results in slower movement.
  return glm::normalize(glm::cross(front_, world_up_));
}

void Camera::ProcessKeyboardInput(bool forward, bool backward, bool left,
                                  bool right, double delta_time) {
  const auto velocity =
      static_cast<float>(movement_speed_ * delta_time * 1000.0);
  if (forward)
    position_ += front_ * velocity;
  if (backward)
    position_ -= front_ * velocity;
  if (left)
    position_ -= right_ * velocity;
  if (right)
    position_ += right_ * velocity;
}

}  // namespace engine
