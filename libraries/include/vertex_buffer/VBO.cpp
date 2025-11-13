#define GLFW_INCLUDE_NONE
#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include "./VBO.h"


template <typename T>
VBO<T>::VBO(T* vertices , GLsizeiptr size , GLuint usage){
    glGenBuffers(1, &ID);
    glBindVertexArray(ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, usage);
}


template <typename T>
void VBO<T>::Bind(){
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}


template <typename T>
void VBO<T>::Unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

template <typename T>
void VBO<T>::Delete(){
    glDeleteBuffers(1 , ID);
}

