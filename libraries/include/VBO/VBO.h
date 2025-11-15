#ifndef VBO_H
#define VBO_H

#define GLFW_INCLUDE_NONE
#include "../glad/glad.h"
#include <GLFW/glfw3.h>


class VBO{
public:
    GLuint ID;
    VBO(GLfloat* vertices , GLsizeiptr size , GLuint usage);
    explicit VBO(GLint* vertices , GLsizeiptr size , GLuint usage);

    void Bind();
    void Unbind();
    void Delete();

};

#endif
