#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//// enum for tracking movement
enum CameraMovement{
    UP,
    DOWN,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//// default camera attributes
const glm::vec3 CAM_POS         = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 CAM_FRONT       = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 CAM_UP          = glm::vec3(0.0, 1.0f, 0.0f);
const float FOV                = 45.0f;
const float YAW                 = -90.0f;
const float PITCH               = 0.0f;

//// camera settings
const float mouseSensitivty     = 0.1f;
const float moveSpeed           = 2.25f;

class Camera
{
    public:
    // camera attributes
    glm::vec3 camPos;
    glm::vec3 camUp;
    glm::vec3 worldUp;
    glm::vec3 camFront;
    glm::vec3 camRight;
    float pitch, yaw;
    float fov = FOV;

    // constructor
    Camera(glm::vec3 pos = CAM_POS, glm::vec3 up = CAM_UP, float ya = YAW, float pi = PITCH) 
    {
        camPos = pos;
        worldUp = up;
        yaw = ya;
        pitch = pi;

        updateCameraVecs();
    }

    // gets the view matrix
    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(camPos, camPos + camFront, camUp);
    }

    float getFOV()
    {
        return fov;
    }

    // handles camera movement
    void doCameraMovement(CameraMovement direction, float deltaTime)
    {
        float velocity = moveSpeed * deltaTime;

        switch(direction)
        {
            case CameraMovement::FORWARD:
                camPos += velocity * camFront;
                break;
            case CameraMovement::BACKWARD:
                camPos -= velocity * camFront;
                break;
            case CameraMovement::LEFT:
                camPos -= camRight * velocity;
                break;
            case CameraMovement::RIGHT:
                camPos += camRight * velocity;
                break;
            case CameraMovement::UP:
                camPos += velocity * camUp;
                break;
            case CameraMovement::DOWN:
                camPos -= velocity * camUp;
                break;
        }
    }

    // handle mouse scrolling
    void doScroll(double yoffset)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f) {fov = 1.0f;}
        if (fov > 45.0f) {fov = 45.0f;}
    }

    // handle mouse movement
    void doMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= mouseSensitivty;
        yoffset *= mouseSensitivty;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) {pitch = 89.0f;}
        if (pitch < -89.0f) {pitch = -89.0f;}

        updateCameraVecs();
    }

    private:
    // re-calcs the cam vectors
    void updateCameraVecs()
    {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camFront = glm::normalize(direction);

        camRight = glm::normalize(glm::cross(camFront, worldUp));
        camUp = glm::normalize(glm::cross(camRight, camFront));
    }
};

#endif