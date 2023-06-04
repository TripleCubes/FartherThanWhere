#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
public:
    Texture();
    
    void init(const std::string &path, bool isLinearFilter = false);
    unsigned int getTextureId() const;
    int getWidth() const;
    int getHeight() const;
    void release();

    Texture(const Texture&) = delete;
    Texture &operator = (const Texture&) = delete;
    
private:
    int width, height, numberOfColorChannels;
    unsigned int textureId;
    bool initialized = false;
    bool released = false;
};

#endif
