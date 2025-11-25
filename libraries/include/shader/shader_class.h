#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glm/ext/matrix_float4x4.hpp>
#define GLFW_INCLUDE_NONE
#include <string>
#include <GLFW/glfw3.h>
#include "../glad/glad.h"
#include <glm/glm.hpp>


std::string getFileContent(const char * filepath);

class Shader{
public:

    GLint viewMatrixLocation = -1;
    GLuint ID;

public:

    Shader();
    Shader(const char * vertexShader , const char * fragmentShader);
    void setViewMatrix(GLfloat* projectionMatrixDP , GLfloat* viewMatrixDP);
    void setTextureCubeMap();
    void Refresh(const char * vertexShader , const char * fragmentShader);
    void Activate();
    void Delete();
};

#endif
