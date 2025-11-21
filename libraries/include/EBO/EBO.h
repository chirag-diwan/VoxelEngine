#ifndef EBO_H
#define EBO_H

#define GLFW_INCLUDE_NONE
#include "../glad/glad.h"
#include <GLFW/glfw3.h>


class EBO{
public:
    GLuint ID;
    EBO();
    EBO(GLuint* indices , GLsizeiptr size , GLuint usage);
    void Refresh(const void* data, size_t size, GLenum usage) ;
    void Bind();
    void Unbind();
    void Delete();

};

#endif
