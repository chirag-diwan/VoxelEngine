#include "./FBO.h"

FBO::FBO() : ID(0){}

void FBO::Refresh(){
    if(ID == 0){
        glGenFramebuffers(1 , &ID);
    }
}

void FBO::LinkTexture(GLuint TextureID){
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, TextureID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    Unbind();
}

void FBO::Bind(){
    glBindFramebuffer(GL_FRAMEBUFFER , ID);
}

void FBO::Unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER , 0);
}
