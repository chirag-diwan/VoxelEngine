#include "./camera.h"
#include <glm/ext/matrix_transform.hpp>

void Camera::setView(){
    view = glm::lookAt(CameraPos, target, up);
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
