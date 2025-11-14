#include <cstdlib>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../libraries/include/glad/glad.h"
#include "../libraries/include/VBO/VBO.h"
#include "../libraries/include/VAO/VAO.h"
#include "../libraries/include/EBO/EBO.h"
#include "../libraries/include/shader/shader_class.h"
#include "../libraries/include/camera/camera.h"
#include <glm/gtc/type_ptr.hpp>

int main() {
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f  
    };

    // 6

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        0, 3, 7,
        7, 4, 0,

        1, 5, 6,
        6, 2, 1,

        0, 1, 5,
        5, 4, 0,

        3, 2, 6,
        6, 7, 3
    };




    VAO vao;
    vao.Bind();

    VBO vbo(vertices, sizeof(vertices), GL_STATIC_DRAW);
    EBO ebo(indices , sizeof(indices) , GL_STATIC_DRAW);

    vao.LinkVbo(vbo, 0);
    vbo.Unbind();
    vao.Unbind();

    Shader shader("shaders/default.vert" , "shaders/default.frag");
    shader.Activate();

    Camera camera;
    glm::mat4& view = camera.getView();
    glm::mat4& projection = camera.getProjection();


    glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);

    shader.setViewMatrix(glm::value_ptr(projection), glm::value_ptr(view));


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader.ID);
        vao.Bind();
        glDrawElements(GL_TRIANGLES , sizeof(indices)/sizeof(indices[0]),GL_UNSIGNED_INT , 0);
        vao.Unbind();
        glfwSwapBuffers(window);
    }
    ebo.Delete();
    vao.Delete();
    vbo.Delete();
    shader.Delete();


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
