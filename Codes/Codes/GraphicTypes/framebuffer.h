#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>

class Framebuffer {
public:
    Framebuffer();
    
    void init(int width = 0, int height = 0, int numberOfTextures = 1);
    void bind() const;
    unsigned int getFBO() const;
    unsigned int getTextureId(int index = 0) const;
    // void resize(int width = 0, int height = 0);
    void release();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer &operator = (const Framebuffer&) = delete;
    
private:
    unsigned int FBO;
    std::vector<unsigned int> textureIdList;
    unsigned int RBO;
    bool initialized = false;
    bool released = false;
};

#endif
