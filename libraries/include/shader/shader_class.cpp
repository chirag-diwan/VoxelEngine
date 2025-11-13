#include "shader_class.h"
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <fstream>
#include <string>
#include <GLFW/glfw3.h>
#include "../glad/glad.h"


std::string getFileContent(const char * filepath){
    std::ifstream file(filepath);
    std::string line;
    std::string fileContent;
    while(std::getline(file , line)){
        fileContent += line;
    }
    return fileContent;
}


Shader::Shader(const char * vertexShader , const char * fragmentShader){
    std::string buffer = getFileContent(vertexShader);
    const char * vertexShaderContent = buffer.c_str();
    buffer = getFileContent(fragmentShader);
    const char * fragmentShaderContent = buffer.c_str();
    

    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(VertexShader, 1, &vertexShaderContent, NULL);
    glShaderSource(FragmentShader, 1, &fragmentShaderContent, NULL);

    glCompileShader(VertexShader);
    glCompileShader(FragmentShader);


    GLint success;
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex Shader compilation failed: " << infoLog << std::endl;
    }
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment Shader compilation failed: " << infoLog << std::endl;
    }


    ID = glCreateProgram();
    glAttachShader(ID, VertexShader);
    glAttachShader(ID, FragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}


void Shader::Activate(){
    glUseProgram(ID);
}


void Shader::Delete(){
    glDeleteProgram(ID);
}

