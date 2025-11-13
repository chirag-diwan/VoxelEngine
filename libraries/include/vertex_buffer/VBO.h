#ifndef VBO_H
#define VBO_H

#define GLFW_INCLUDE_NONE
#include "../glad/glad.h"
#include <GLFW/glfw3.h>


template <typename T>
class VBO{
public:
    GLuint ID;
    VBO(T* vertices , GLsizeiptr size , GLuint usage);

    void Bind();
    void Unbind();
    void Delete();

};

#endif
