#ifndef CAMERA_H
#define CAMERA_H

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

    glm::vec3 CameraPos = {1,1,1};
    glm::vec3 target = {0,0,0};
    glm::vec3 up = {0,0,1};
    float fov = 45;
    float aspectRatio = 16.f/9.f;
    float nearPlane = 1.0f;
    float farPlane = 100.0f;

public:

    void setView();
    void setProjection();
    void advance(glm::vec3 Target);
    glm::mat4& getView();
    glm::mat4& getProjection();
    
};



#endif
