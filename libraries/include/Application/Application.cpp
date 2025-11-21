#include "./Application.h"
#include <iostream>

Application::Application() : deltaTime(0.0f) {

    vertices.reserve(1000000);  
    indices.reserve(1500000);
}

Application::~Application() {
    _ebo.Delete();
    _vbo.Delete();
    _vao.Delete();
    shader.Delete();
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

}

void Application::GenerateWorld() {
    world.fetchMergedMesh(vertices, indices);
}

bool Application::Initialize() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return true;
}

bool Application::SetWindow() {
    window = glfwCreateWindow(800, 600, "Voxel", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        return false;
    }
    return true;
}

bool Application::SetBuffers() {
    if (vertices.empty()) {
        std::cerr << "No vertices for buffers\n";
        return false;
    }
    _vao.Refresh();
    _vao.Bind();
    _vbo.Refresh(vertices.data(), vertices.size() * sizeof(Vertex), GL_DYNAMIC_DRAW);
    _ebo.Refresh(indices.data(), indices.size() * sizeof(GLuint), GL_DYNAMIC_DRAW);  



    _vao.LinkIntVbo(_vbo, 0, 3, 6, (void*)0);  
    _vao.LinkIntVbo(_vbo, 1, 3, 6, (void*)(3 * sizeof(float)));  
    _vbo.Unbind();
    _vao.Unbind();
    return true;
}

bool Application::SetShaders() {
    if (!window) return false;
    shader.Refresh("shaders/default.vert", "shaders/default.frag");
    if (!shader.ID) {
        std::cerr << "Shader compilation failed\n";
        return false;
    }
    shader.Activate();
    return true;
}

bool Application::SetCamera() {
    camera.setProjection();
    camera.setView();

    shader.setViewMatrix(glm::value_ptr(camera.getProjection()), glm::value_ptr(camera.getView()));
    return true;
}

void Application::Run() {
    if (!window) return;


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    InputHandler ih(window, &camera);
    float lastTime = 0.0f;
    float currentTime = 0.0f;
    glm::ivec3 lastCamChunk = glm::ivec3(999);
    bool meshNeedsUpdate = true;  


    world.ChunkManager(camera.CameraPos, renderDistance);
    GenerateWorld();
    SetBuffers();

    while (!glfwWindowShouldClose(window)) {
        currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glfwPollEvents();
        ih.processKeyPress(deltaTime);

        glm::ivec3 currCamChunk = glm::floor(camera.CameraPos / static_cast<float>(CHUNK_SIZE));
        if (currCamChunk != lastCamChunk) {
            world.ChunkManager(camera.CameraPos, 5);
            meshNeedsUpdate = true;  
            lastCamChunk = currCamChunk;
        }


        if (meshNeedsUpdate) {
            GenerateWorld();
            SetBuffers();
            meshNeedsUpdate = false;
        }


        shader.setViewMatrix(glm::value_ptr(camera.getProjection()), glm::value_ptr(camera.getView()));

        glClearColor(0.85f, 0.85f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader.ID);
        _vao.Bind();
        if (!indices.empty()) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        }
        _vao.Unbind();

        glfwSwapBuffers(window);
    }
}
