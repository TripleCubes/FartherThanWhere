#include <Codes/GraphicTypes/framebuffer.h>

#include <glad/glad.h>
#include <Codes/print.h>

extern int currentWindowWidth;
extern int currentWindowHeight;

Framebuffer::Framebuffer() {}

void Framebuffer::init(int width, int height) {
    if (initialized) {
        return;
    }

    if (width == 0) {
        width = currentWindowWidth;
    }
    if (height == 0) {
        height = currentWindowHeight;
    }

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &textureId);
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);  



    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); 

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        PRINTLN("cant init frame buffer");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initialized = true;
}

void Framebuffer::bind() const {
    if (!initialized) {
        PRINTLN("cant bind uninitialized frame buffer");
        return;
    }
    if (released) {
        PRINTLN("cant bind released frame buffer");
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

unsigned int Framebuffer::getFBO() const {
    if (!initialized) {
        PRINTLN("cant get FBO of uninitialized frame buffer");
        return 0;
    }
    if (released) {
        PRINTLN("cant get FBO of released frame buffer");
        return 0;
    }
    return FBO;
}

unsigned int Framebuffer::getTextureId() const {
    if (!initialized) {
        PRINTLN("cant get textureId of uninitialized frame buffer");
        return 0;
    }
    if (released) {
        PRINTLN("cant get textureId of released frame buffer");
        return 0;
    }
    return textureId;
}

void Framebuffer::resize(int width, int height) {
    if (!initialized) {
        PRINTLN("cant resize uninitialized frame buffer");
        return;
    }
    if (released) {
        PRINTLN("cant resize released frame buffer");
        return;
    }

    if (width == 0) {
        width = currentWindowWidth;
    }
    if (height == 0) {
        height = currentWindowHeight;
    }
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::release() {
    if (released) {
        return;
    }
    
    if (!initialized) {
        PRINTLN("cant release uninitialized frame buffer");
        return;
    }

    glDeleteTextures(1, &textureId);
    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &FBO);  

    released = true;
}
