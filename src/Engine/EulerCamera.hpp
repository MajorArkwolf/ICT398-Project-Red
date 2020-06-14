#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>


/// Makes understanding the direction simple.
enum class Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

/// Default values used to initialise a camera.
constexpr float YAW         = 0.0;
constexpr float PITCH        = 0.0;
constexpr float SPEED        = 0.05;
constexpr float SENSITIVITY  = 0.1;
constexpr float ZOOM         = 45.0;
namespace View {
/**
 * Euler Camera designed to interact in a 3D environment.
 */
    class Camera {
    public:
        /// Camera Attributes
        glm::vec3 Position = {};
        glm::vec3 Front = {};
        glm::vec3 Up = {};
        glm::vec3 Right = {};
        glm::vec3 WorldUp = {};
        /// Euler Angles
        float Yaw = {};
        float Pitch = {};
        /// Camera options
        float MovementSpeed = {};
        float MouseSensitivity = {};
        double Zoom = {};

        /**
         * Default Constructor
         * @param position the position of the camera.
         * @param up The up axis
         * @param yaw of the camera.
         * @param pitch of the camera.
         */
        Camera(glm::dvec3 position = glm::dvec3(0.0f, 0.0f, 0.0f),
               glm::dvec3 up = glm::dvec3(0.0f, 1.0f, 0.0f), double yaw = YAW,
               double pitch = PITCH);

        /**
         * Overloaded constructor
         * @param posX the position on the x axis.
         * @param posY the position on the y axis.
         * @param posZ the position on the z axis.
         * @param upX position on the x axis.
         * @param upY position on the y axis.
         * @param upZ position on the z axis.
         * @param yaw of the camera.
         * @param pitch of the camera.
         */
        Camera(double posX, double posY, double posZ, double upX, double upY,
               double upZ, double yaw, double pitch);

        /**
         * Calculates the view matrix used for shaders and the rendering engine.
         * @return Gets the view matrix
         */
        glm::mat4 GetViewMatrix();

        /**
         * Applies the movement based on the default values and delta time.
         * @param direction input received from any keyboard-like input system
         * @param deltaTime produced by the engine.
         */
        void ProcessKeyboard(Camera_Movement direction, double deltaTime);


        /**
         * Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
         * @param xoffset the mouses x offset.
         * @param yoffset the mouse y offset
         * @param constrainPitch ensures the camera cant be flipped.
         */
        void ProcessMouseMovement(double xoffset, double yoffset,
                                  GLboolean constrainPitch = true);

        /**
         * Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
         * @param yoffset the offset of the mouse.
         */
        void ProcessMouseScroll(double yoffset);

        /**
         * Gets the location of the camera on the x and y axis.
         * @return
         */
        glm::ivec2 getLocation() const;

        glm::vec3 GetRightVector();

        void ProcessKeyboardInput(bool forward, bool backward, bool left, bool right, double deltaTime);
        static void LuaInit();
        /**
         * Calculates the front vector from the Camera's (updated) Euler Angles
         */
        void updateCameraVectors();

    private:


    };
}
