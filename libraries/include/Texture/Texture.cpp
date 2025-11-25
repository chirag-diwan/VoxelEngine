#include "./Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../STB/STB.h"

Texture::Texture() : ID(0) {}

void Texture::Refresh() {
    if (ID == 0) {
        glGenTextures(1, &ID);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture::LinkCubeMap(const char* const filePaths[], size_t size) {
    Bind();  

    int width = 0, height = 0, nrChannels = 0;
    unsigned char* data = nullptr;
    bool allLoaded = true;  

    for (size_t i = 0; i < size; ++i) {
        data = stbi_load(filePaths[i], &width, &height, &nrChannels, 0);
        if (data != nullptr) {
            GLenum internalFormat = (nrChannels == 4) ? GL_RGBA : GL_RGB;  
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLint>(i),  
                0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        } else {
            std::cerr << "Unable to load cubemap face: " << filePaths[i] << std::endl;
            allLoaded = false;
        }
    }

    if (!allLoaded) {
        std::cerr << "Warning: Cubemap is incomplete! Skybox may appear distorted or black." << std::endl;
    }

    warpAndFilter();  
}

void Texture::warpAndFilter() {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
