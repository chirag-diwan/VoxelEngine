#include "./camera.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>


void Camera::setView(){
    view = glm::lookAt(CameraPos , CameraPos + front , up);
}

void Camera::setProjection() {
    projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

glm::mat4& Camera::getView() {
    return view;
}

glm::mat4& Camera::getProjection() {
    return projection;
}

void Camera::processMouseMove(float xpos, float ypos)
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(direction);
}

void Camera::processKeyInput(int key , float deltaTime){
    CameraRight = glm::normalize(glm::cross(front , up));
    CameraUp = glm::cross(CameraRight , front);
    switch (key) {
        case GLFW_KEY_W:
            CameraPos += front*cameraSpeed*deltaTime;
            break;
        case GLFW_KEY_A:
            CameraPos -= CameraRight*cameraSpeed*deltaTime;
            break;
        case GLFW_KEY_S:
            CameraPos -= front*cameraSpeed*deltaTime;
            break;
        case GLFW_KEY_D:
            CameraPos += CameraRight*cameraSpeed*deltaTime;
            break;
    }
}
