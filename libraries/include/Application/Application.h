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
#include "../Texture/Texture.h"
#include "../Light/Light.h"


class Application {
private:
    GLFWwindow* window = nullptr;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;  
    Texture skyCubeMap;
    Camera camera;
    Shader shader;
    World world;
    VAO _vao;
    VBO _vbo;
    EBO _ebo;
    VBO _skyVbo;
    VAO _skyVao;
    float deltaTime;
    int renderDistance = 15;
    float Gravity = 1;
    Light globalLight;
    static constexpr GLfloat skyVerts[] = {
        // Scale factor: 500.0f (tune: match your projection far plane / 2)
        -500,  500, -500,
        -500, -500, -500,
        500, -500, -500,
        500, -500, -500,
        500,  500, -500,
        -500,  500, -500,
        -500, -500,  500,
        -500, -500, -500,
        -500,  500, -500,
        -500,  500, -500,
        -500,  500,  500,
        -500, -500,  500,
        500, -500, -500,
        500, -500,  500,
        500,  500,  500,
        500,  500,  500,
        500,  500, -500,
        500, -500, -500,
        -500, -500,  500,
        -500,  500,  500,
        500,  500,  500,
        500,  500,  500,
        500, -500,  500,
        -500, -500,  500,
        -500,  500, -500,
        500,  500, -500,
        500,  500,  500,
        500,  500,  500,
        -500,  500,  500,
        -500,  500, -500,
        -500, -500, -500,
        -500, -500,  500,
        500, -500, -500,
        500, -500, -500,
        -500, -500,  500,
        500, -500,  500
    };
    static constexpr const char * CubeMapPath[] = {
        "CubeMap/right.png",
        "CubeMap/left.png",
        "CubeMap/top.png",
        "CubeMap/bottom.png",
        "CubeMap/front.png",
        "CubeMap/back.png",
    };

public:
    Application() ;
    ~Application() ;
    void GenerateWorld();
    bool Initialize() ;
    bool SetWindow() ;
    bool SetBuffers() ;
    void SetTexture() ;
    void setCubeMap();
    bool SetShaders() ;
    bool SetCamera() ;
    void Run() ;
};

#endif 
