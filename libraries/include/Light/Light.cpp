#include "./Light.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

Light::Light(){}

void Light::UpdatePosition(float time){
    time = time*reductionFactor;
    Position = {glm::sin(time) , glm::cos(time) , 0};
}

void Light::SetView(){
    View = glm::lookAt(Position,{0,0,0}, {0,1,0});
}

glm::vec3& Light::GetPosition(){
    return Position;
}

void Light::SetProjection() {
    Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane); 
}

glm::mat4& Light::getView() {
    return View;
}

glm::mat4& Light::getProjection() {
    return Projection;
}
