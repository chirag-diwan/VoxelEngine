#include "./Application.h"

Application::Application(){}

Application::~Application() { 
    _ebo.Delete();
    _vbo.Delete();
    _vao.Delete();
    shader.Delete();
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    std::cout << 1.0/deltaTime;
    std::cin.ignore();
}

void Application::GenerateWorld(){
    world.setBlocks({0, 0, 0});
    world.generateMesh();
    vertices = world.getVerticesRefrence();  
    indices = world.getIndicesRefrence();
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
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
    _vbo.Refresh(vertices.data(), vertices.size()*sizeof(Vertex), GL_DYNAMIC_DRAW);
    _ebo.Refresh(indices.data() , indices.size()*sizeof(indices[0]) , GL_DYNAMIC_DRAW);
    _vao.LinkIntVbo(_vbo, 0, 3, 6, (void*)0);     
    _vao.LinkIntVbo(_vbo, 1, 3, 6, (void*)(3 * sizeof(int)));
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

    while (!glfwWindowShouldClose(window)) {
        currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glfwPollEvents();
        ih.processKeyPress(deltaTime);

        shader.setViewMatrix(glm::value_ptr(camera.getProjection()), glm::value_ptr(camera.getView()));

        glClearColor(0.85f, 0.85f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader.ID);
        _vao.Bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        _vao.Unbind();

        glfwSwapBuffers(window);
    }
}
