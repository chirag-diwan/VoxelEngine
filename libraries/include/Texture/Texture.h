#ifndef TEXTURE_H
#define TEXTURE_H
#include "../glad/glad.h"
#include <cstddef>

class Texture{
private:
    GLuint ID;
public:
    Texture();
    void Refresh();
    void Bind();
    void Unbind();
    void LinkCubeMap(const char * const filePaths[], size_t size);
    void warpAndFilter();
    void Generate2DDepthMap(unsigned int height , unsigned int width);
};


#endif
