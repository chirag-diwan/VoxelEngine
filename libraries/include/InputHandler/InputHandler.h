#ifndef INPUT_HANDLER
#define INPUT_HANDLER
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../glad/glad.h"
#include "../camera/camera.h"


class InputHandler{
private:

    static bool keys[1024];
    static float mouseX;
    static float mouseY;
    static float lastX;
    static float lastY;
    static Camera* camera;

private:

    static void keyCallBack(GLFWwindow* window , int key , int scanCode , int action , int mods);
    static void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

public:

    InputHandler(GLFWwindow* window , Camera* camera);
    void processKeyPress(float deltatime);

};

#endif
