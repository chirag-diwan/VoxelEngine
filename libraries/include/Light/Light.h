#ifndef LIGHT_H
#define LIGHT_H

#include <GL/gl.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

class Light{
private:
    glm::mat4 View;
    glm::mat4 Projection;
    glm::vec3 Position;
    GLfloat nearPlane = 1.0f;
    GLfloat farPlane = 100.0f;
    float reductionFactor = 0.001;
public:
    Light();
    void SetView();
    void SetProjection();
    void UpdatePosition(float time);
    glm::vec3& GetPosition();
    glm::mat4& getView();
    glm::mat4& getProjection();
};

#endif
