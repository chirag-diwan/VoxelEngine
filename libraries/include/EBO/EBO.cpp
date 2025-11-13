#define GLFW_INCLUDE_NONE
#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include "./EBO.h"


template <typename T>
EBO<T>::EBO(T* vertices , GLsizeiptr size , GLuint usage){
    glGenBuffers(1, &ID);
    glBindVertexArray(ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, usage);
}


template <typename T>
void EBO<T>::Bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}


template <typename T>
void EBO<T>::Unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

template <typename T>
void EBO<T>::Delete(){
    glDeleteBuffers(1 , ID);
}

