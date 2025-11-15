#include "VAO.h"

void VAO::LinkFloatVbo(VBO& vbo, GLuint index, GLint components, GLsizei strideFloats, void* offset)
{
    vbo.Bind();
    glVertexAttribPointer(
        index, 
        components, 
        GL_FLOAT, 
        GL_FALSE, 
        strideFloats * sizeof(float), 
        offset
    );
    glEnableVertexAttribArray(index);
    vbo.Unbind();
}

void VAO::LinkIntVbo(VBO& vbo, GLuint index, GLint components, GLsizei strideFloats, void* offset)
{
    vbo.Bind();
    glVertexAttribPointer(
        index, 
        components, 
        GL_INT, 
        GL_FALSE, 
        strideFloats * sizeof(int), 
        offset
    );
    glEnableVertexAttribArray(index);
    vbo.Unbind();
}


VAO::VAO(){
    glGenVertexArrays(1, &ID);
}

void VAO::Bind(){
    glBindVertexArray(ID);
}

void VAO::Unbind(){
    glBindVertexArray(0);
}

void VAO::Delete(){
    glDeleteVertexArrays(1, &ID);
}



