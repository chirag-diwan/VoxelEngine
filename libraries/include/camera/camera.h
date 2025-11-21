#ifndef CAMERA_H
#define CAMERA_H
#define  GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int3_sized.hpp>
#include <glm/ext/vector_int4_sized.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

class Camera{
private:
    glm::mat4 view;
    glm::mat4 projection;

public:

    glm::vec3 CameraPos = {0,20,0};
    glm::vec3 target = {0,0,0};
    glm::vec3 up = {0,1,0};
    glm::vec3 CameraRight = {1,0,0};
    glm::vec3 front = glm::vec3(0,0,-1) ;
    
    glm::vec3 CameraUp = {0,1,0};


    float fov = 45;
    float aspectRatio = 16.f/9.f;
    float nearPlane = 1.0f;
    float farPlane = 1000.0f;

    float yaw   = -90.0f;   
    float pitch = 0.0f;

    float lastX = 400;   
    float lastY = 300;

    bool firstMouse = true;

    float sensitivity = 0.2f;
    
    float cameraSpeed = 3.0f;

public:

    void setView();
    void setProjection();
    void advance(glm::vec3 Target);
    glm::mat4& getView();
    glm::mat4& getProjection();
    void processMouseMove(float x , float y);
    void processKeyInput(int key , float deltaTime);
};



#endif
