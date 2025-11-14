#include <cstdlib>
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../libraries/include/glad/glad.h"
#include "../libraries/include/VBO/VBO.h"
#include "../libraries/include/VAO/VAO.h"
#include "../libraries/include/EBO/EBO.h"
#include "../libraries/include/shader/shader_class.h"

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
        -0.5f, 0.5f, 0.0f,
         0.4f, 0.1f, 0.0f,
         0.0f, 0.0f, 0.0f
    };

    VAO vao;
    vao.Bind();
    VBO vbo(vertices, sizeof(vertices), GL_STATIC_DRAW);
    Shader shader("shaders/default.vert" , "shaders/default.frag");
    shader.Activate();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader.ID);
        glBindVertexArray(vao.ID);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }
    vao.Delete();
    vbo.Delete();
    shader.Delete();
    

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
