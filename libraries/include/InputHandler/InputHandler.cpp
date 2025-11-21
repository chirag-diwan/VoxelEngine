#include "./InputHandler.h"
#include <GLFW/glfw3.h>

bool  InputHandler::keys[1024] = { false };

float InputHandler::mouseX = 0.0f;
float InputHandler::mouseY = 0.0f;
float InputHandler::lastX  = 0.0f;
float InputHandler::lastY  = 0.0f;

Camera* InputHandler::camera = nullptr;

InputHandler::InputHandler(GLFWwindow* window , Camera* cam){
    camera = cam;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetKeyCallback(window, keyCallBack);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void InputHandler::keyCallBack(GLFWwindow* window , int key , int scanCode , int action , int mods){
    if(action == GLFW_PRESS){
        keys[key] = true;
    }else if(action == GLFW_RELEASE){
        keys[key] = false;
    }
}

void InputHandler::mousePosCallback(GLFWwindow* window, double xpos, double ypos){
    mouseX = xpos;
    mouseY = ypos;
    camera->processMouseMove(mouseX, mouseY);
}

void InputHandler::processKeyPress(float deltaTime){
    if(keys[GLFW_KEY_W])
        camera->processKeyInput(GLFW_KEY_W, deltaTime);
    if(keys[GLFW_KEY_A])
        camera->processKeyInput(GLFW_KEY_A, deltaTime);
    if(keys[GLFW_KEY_S])
        camera->processKeyInput(GLFW_KEY_S, deltaTime);
    if(keys[GLFW_KEY_D])
        camera->processKeyInput(GLFW_KEY_D, deltaTime);
    camera->setProjection();
    camera->setView();
}

void InputHandler::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    camera->aspectRatio = (float)width/height;
    camera->setView();
    camera->setProjection();
}

