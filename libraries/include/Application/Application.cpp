#include "./Application.h"
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

Application::Application() : deltaTime(0.0f) {
    vertices.reserve(10000000);  
    indices.reserve(15000000);
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


void Application::SetTexture(){
    skyCubeMap.Refresh();
    skyCubeMap.LinkCubeMap(CubeMapPath,6);
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


void Application::GenerateWorld() {
    world.fetchMergedMesh(vertices, indices);
}

bool Application::SetBuffers() {
    if (vertices.empty()) {
        std::cerr << "No vertices for buffers\n";
        return false;
    }


    if (_vao.ID != 0) _vao.Delete();
    _vao.Refresh();
    _vao.Bind();

    if (_vbo.ID != 0) _vbo.Delete();
    if (_ebo.ID != 0) _ebo.Delete();
    _vbo.Refresh(vertices.data(), vertices.size() * sizeof(Vertex), GL_DYNAMIC_DRAW);
    _ebo.Refresh(indices.data(), indices.size() * sizeof(GLuint), GL_DYNAMIC_DRAW);

    _vao.LinkIntVbo(_vbo, 0, 3, 6, (void*)0);                       
    _vao.LinkIntVbo(_vbo, 1, 3, 6, (void*)(3 * sizeof(int)));       

    _vbo.Unbind();
    _vao.Unbind();


    if (_skyVao.ID != 0) _skyVao.Delete();
    _skyVao.Refresh();
    _skyVao.Bind();

    if (_skyVbo.ID != 0) _skyVbo.Delete();
    _skyVbo.Refresh((GLfloat*)skyVerts, sizeof(skyVerts), GL_DYNAMIC_DRAW);  

    _skyVao.LinkFloatVbo(_skyVbo, 2, 3, 3, (void*)0);  


    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    _skyVbo.Unbind();
    _skyVao.Unbind();

    return true;
}

void Application::Run() {
    if (!window) return;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    InputHandler ih(window, &camera);
    float lastTime = 0.0f;
    float currentTime = 0.0f;
    glm::ivec3 lastCamChunk = glm::ivec3(999);
    bool meshNeedsUpdate = true;

    world.ChunkManager(camera.CameraPos, renderDistance);
    GenerateWorld();
    SetBuffers();

    auto skyBoxLoc = glGetUniformLocation(shader.ID, "skybox");
    auto isSkyBoxLoc = glGetUniformLocation(shader.ID, "isSkyBox");
    auto sunNormalLoc = glGetUniformLocation(shader.ID , "aSunNormal");
    auto LightViewLoc = glGetUniformLocation(shader.ID , "aLightView");
    auto LightProjection = glGetUniformLocation(shader.ID , "aLightProjection");
    auto LightPassLoc = glGetUniformLocation(shader.ID , "isLightPass");

    globalLight.UpdatePosition(0);
    glm::vec3 lightPosition;
    while (!glfwWindowShouldClose(window)) {
        currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        globalLight.UpdatePosition(lastTime);
        lightPosition = globalLight.GetPosition();
        glfwPollEvents();
        ih.processKeyPress(deltaTime);
        //globalLight.SetView();
        //globalLight.SetProjection();

        glm::ivec3 currCamChunk = glm::floor(camera.CameraPos / static_cast<float>(CHUNK_SIZE));
        if (currCamChunk != lastCamChunk) {
            world.ChunkManager(camera.CameraPos, renderDistance);
            meshNeedsUpdate = true;
            lastCamChunk = currCamChunk;
        }

        if (meshNeedsUpdate) {
            GenerateWorld();
            SetBuffers();
            meshNeedsUpdate = false;
        }


        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader.ID);  


        shader.setViewMatrix(glm::value_ptr(camera.getProjection()), glm::value_ptr(camera.getView()));


        glDepthFunc(GL_LEQUAL);
        glUniform1i(skyBoxLoc, 0);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(isSkyBoxLoc, 1);
        glUniform3f(sunNormalLoc,lightPosition.x , lightPosition.y , lightPosition.z);
        skyCubeMap.Bind();
        _skyVao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        _skyVao.Unbind();
        glUniform1i(isSkyBoxLoc, 0);  


        glDepthFunc(GL_LEQUAL);
        _vao.Bind();
        if (!indices.empty()) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        }
        _vao.Unbind();

        glfwSwapBuffers(window);
    }
}
