#define GLFW_INCLUDE_NONE
#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include "./VBO.h"


VBO::VBO(GLfloat* vertices , GLsizeiptr size , GLuint usage){
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}

VBO::VBO(GLint* vertices , GLsizeiptr size , GLuint usage){
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}


void VBO::Bind(){
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}


void VBO::Unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete(){
    glDeleteBuffers(1 , &ID);
}

