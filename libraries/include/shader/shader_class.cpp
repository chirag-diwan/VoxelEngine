#include "shader_class.h"
#include <cstdlib>
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <fstream>
#include <string>
#include <GLFW/glfw3.h>
#include "../glad/glad.h"


std::string getFileContent(const char * filepath)
{
    std::ifstream file(filepath);
    if(file.fail())
    {
        std::cerr << "Error Opening file :: " << filepath;
        std::exit(EXIT_FAILURE);
    }
    std::string line;
    std::string fileContent;
    while(std::getline(file , line))
    {
        fileContent += line + "\n";
    }
    return fileContent;
}


Shader::Shader()
{}

Shader::Shader(const char * vertexShader , const char * fragmentShader)
{
    std::string buffer = getFileContent(vertexShader);
    const char * vertexShaderContent = buffer.c_str();
    std::string buffer2 = getFileContent(fragmentShader);
    const char * fragmentShaderContent = buffer2.c_str();


    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(VertexShader, 1, &vertexShaderContent, NULL);
    glShaderSource(FragmentShader, 1, &fragmentShaderContent, NULL);

    glCompileShader(VertexShader);
    glCompileShader(FragmentShader);


    GLint success;
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex Shader compilation failed: " << infoLog << std::endl;
    }
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment Shader compilation failed: " << infoLog << std::endl;
    }


    ID = glCreateProgram();
    glAttachShader(ID, VertexShader);
    glAttachShader(ID, FragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

void Shader::Refresh(const char * vertexShader , const char * fragmentShader)
{
    std::string buffer = getFileContent(vertexShader);
    const char * vertexShaderContent = buffer.c_str();
    std::string buffer2 = getFileContent(fragmentShader);
    const char * fragmentShaderContent = buffer2.c_str();


    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(VertexShader, 1, &vertexShaderContent, NULL);
    glShaderSource(FragmentShader, 1, &fragmentShaderContent, NULL);

    glCompileShader(VertexShader);
    glCompileShader(FragmentShader);


    GLint success;
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex Shader compilation failed: " << infoLog << std::endl;
    }
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment Shader compilation failed: " << infoLog << std::endl;
    }


    ID = glCreateProgram();
    glAttachShader(ID, VertexShader);
    glAttachShader(ID, FragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

}



void Shader::setViewMatrix(GLfloat* projectionMatrixDP , GLfloat* viewMatrixDP)
{
    GLint ViewLocation = glGetUniformLocation(ID , "ViewMatrix");
    GLint ProjectionLocation = glGetUniformLocation(ID , "ProjectionMatrix");
    if(ViewLocation == -1)
    {
        std::cerr << "No Such uniform as ViewMatrix";
        std::exit(EXIT_FAILURE);
    }
    if(ProjectionLocation == -1)
    {
        std::cerr << "No Such uniform as ViewMatrix";
        std::exit(EXIT_FAILURE);
    }
    glUniformMatrix4fv(ViewLocation , 1 , GL_FALSE , viewMatrixDP);
    glUniformMatrix4fv(ProjectionLocation , 1 , GL_FALSE , projectionMatrixDP);

}

void Shader::setTextureCubeMap()
{
    GLint TextureLocation = glGetUniformLocation(ID , "CubeMapTexture");
    if(TextureLocation == -1)
    {
        std::cerr << "No Such Uniform as CubeMapTexture";
    }
}


void Shader::Activate()
{
    glUseProgram(ID);
}


void Shader::Delete()
{
    glDeleteProgram(ID);
}

