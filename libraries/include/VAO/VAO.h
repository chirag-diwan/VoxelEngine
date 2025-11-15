#ifndef VAO_H
#define VAO_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../glad/glad.h"
#include "../VBO/VBO.h"

class VAO{
public:
    GLuint ID;
    VAO();
    void LinkFloatVbo(VBO& vbo, GLuint index, GLint components, GLsizei strideFloats, void* offset);
    void LinkIntVbo(VBO& vbo, GLuint index, GLint components, GLsizei strideFloats, void* offset);

    void Bind();
    void Unbind();
    void Delete();
};

#endif
