#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#define GLFW_INCLUDE_NONE
#include <string>
#include <GLFW/glfw3.h>
#include "../glad/glad.h"


std::string getFileContent(const char * filepath);

class Shader{
public:

    GLuint ID;

public:

    Shader(const char * vertexShader , const char * fragmentShader);
    void Activate();
    void Delete();

};

#endif
