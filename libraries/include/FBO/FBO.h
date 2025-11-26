#ifndef FBO_H
#define FBO_H

#include "../glad/glad.h"

class FBO{
public:
    GLuint ID;
    FBO ();
    void Refresh();
    void Bind();
    void LinkTexture(GLuint TextureID);
    void Unbind();
};

#endif
