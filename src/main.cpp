#define GLM_ENABLE_EXPERIMENTAL
#define GLFW_INCLUDE_NONE
#include <cstdlib>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include "../libraries/include/glad/glad.h"
#include "../libraries/include/VBO/VBO.h"
#include "../libraries/include/VAO/VAO.h"
#include "../libraries/include/EBO/EBO.h"
#include "../libraries/include/shader/shader_class.h"
#include "../libraries/include/camera/camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <sys/types.h>
#include "../libraries/include/glad/glad.h"
#include "../libraries/include/World/World.h"


bool keys[1024];

float x = 0;
float y = 0;

void keyCallBack(GLFWwindow* window , int key , int scanCode , int action , int mods){
    if(action == GLFW_PRESS){
        keys[key] = true;
    }else if(action == GLFW_RELEASE){
        keys[key] = false;
    }
}

void mousePosCallback(GLFWwindow* window, double xpos, double ypos){
    x = xpos;
    y = ypos;
}


float mousex = 0;
float mousey = 0;



int main() {
    World world;
    world.generateMesh();
    auto vertices = world.getVerticesRefrence();
    auto indices = world.getIndicesRefrence();

    std::cout << "Generated verts: " << vertices.size() << ", indices: " << indices.size() << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "Voxel", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetKeyCallback(window, keyCallBack);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }



    VAO vao;
    vao.Bind();

    VBO vbo(vertices.data(), vertices.size()*sizeof(Vertex), GL_DYNAMIC_DRAW);
    EBO ebo(indices.data() , indices.size()*sizeof(indices[0]) , GL_DYNAMIC_DRAW);


    vao.LinkIntVbo(vbo, 0, 3, 6, (void*)0);     
    vao.LinkIntVbo(vbo, 1, 3, 6, (void*)(3 * sizeof(int)));

    vbo.Unbind();
    vao.Unbind();

    Shader shader("shaders/default.vert" , "shaders/default.frag");
    shader.Activate();

    Camera camera;
    camera.setProjection();
    camera.setView();


    glm::mat4& view = camera.getView();
    glm::mat4& projection = camera.getProjection();


    shader.setViewMatrix(glm::value_ptr(projection), glm::value_ptr(view));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);  
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 

    glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);

    float lastTime = 0;
    float currentTime = 0;
    float deltaTime = 0;

    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        glfwPollEvents();
        camera.processMouseMove(x, y);
        if(keys[GLFW_KEY_W])
            camera.processKeyInput(GLFW_KEY_W, deltaTime);
        if(keys[GLFW_KEY_A])
            camera.processKeyInput(GLFW_KEY_A, deltaTime);
        if(keys[GLFW_KEY_S])
            camera.processKeyInput(GLFW_KEY_S, deltaTime);
        if(keys[GLFW_KEY_D])
            camera.processKeyInput(GLFW_KEY_D, deltaTime);

        camera.setProjection();
        camera.setView();
        view = camera.getView();
        projection = camera.getProjection();
        shader.setViewMatrix(glm::value_ptr(projection), glm::value_ptr(view));

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader.ID);
        vao.Bind();
        glDrawElements(GL_TRIANGLES , indices.size(),GL_UNSIGNED_INT , 0);
        vao.Unbind();
        glfwSwapBuffers(window);
        lastTime = currentTime;
    }
    ebo.Delete();
    vao.Delete();
    vbo.Delete();
    shader.Delete();


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
