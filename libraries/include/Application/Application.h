#ifndef APPLICATION_H
#define APPLICATION_H

#define GLM_ENABLE_EXPERIMENTAL
#define GLFW_INCLUDE_NONE

#include <cstdlib>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "../glad/glad.h"
#include "../VBO/VBO.h"
#include "../VAO/VAO.h"
#include "../EBO/EBO.h"
#include "../shader/shader_class.h"
#include "../camera/camera.h"
#include "../World/World.h"
#include "../InputHandler/InputHandler.h"

class Application {
private:
    GLFWwindow* window = nullptr;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;  
    Camera camera;
    Shader shader;
    World world;
    VAO _vao;
    VBO _vbo;
    EBO _ebo;
    float deltaTime;
    int renderDistance = 10;
    float Gravity = 1;


public:
    Application() ;
    ~Application() ;
    void GenerateWorld();
    bool Initialize() ;
    bool SetWindow() ;
    bool SetBuffers() ;
    bool SetShaders() ;
    bool SetCamera() ;
    void Run() ;
};

#endif 
