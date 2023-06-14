#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class Framebuffer {
public:
    Framebuffer();
    
    void init(int width = 0, int height = 0);
    void bind() const;
    unsigned int getFBO() const;
    unsigned int getTextureId() const;
    void resize(int width = 0, int height = 0);
    void release();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer &operator = (const Framebuffer&) = delete;
    
private:
    unsigned int FBO;
    unsigned int textureId;
    unsigned int RBO;
    bool initialized = false;
    bool released = false;
};

#endif
