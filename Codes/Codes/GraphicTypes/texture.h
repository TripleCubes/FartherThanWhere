#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
public:
    Texture();
    
    void init(const std::string &path);
    unsigned int getTextureId() const;
    int getTextureWidth() const;
    int getTextureHeight() const;
    void release();

    Texture(const Texture&) = delete;
    Texture &operator = (const Texture&) = delete;
    
protected:
    int textureWidth, textureHeight, numberOfColorChannels;
    unsigned int textureId;
    bool initialized = false;
    bool released = false;
};

#endif
