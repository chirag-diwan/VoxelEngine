#ifndef EBO_H
#define EBO_H

#define GLFW_INCLUDE_NONE
#include "../glad/glad.h"
#include <GLFW/glfw3.h>


class EBO{
public:
    GLuint ID;
    EBO(GLuint* indices , GLsizeiptr size , GLuint usage);

    void Bind();
    void Unbind();
    void Delete();

};

#endif
