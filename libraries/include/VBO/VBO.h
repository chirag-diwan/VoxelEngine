#ifndef VBO_H
#define VBO_H

#include <glm/ext/vector_int3.hpp>
#define GLFW_INCLUDE_NONE
#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

struct Vertex{
    glm::ivec3 Position;
    glm::ivec3 Normal;
};


class VBO{
public:
    GLuint ID;
    VBO(GLfloat* vertices , GLsizeiptr size , GLuint usage);
    explicit VBO(GLint* vertices , GLsizeiptr size , GLuint usage);
    explicit VBO(Vertex* vertices , GLsizeiptr size , GLuint usage);

    void Bind();
    void Unbind();
    void Delete();

};

#endif
