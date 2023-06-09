#include <Codes/GraphicTypes/texture.h>

#include <glad/glad.h>
#include <stb_image.h>
#include <Codes/Debug/print.h>

Texture::Texture() {}

void Texture::init(const std::string &path) {
    if (initialized) {
        return;
    }

    unsigned char *textureData = stbi_load(path.c_str(), &textureWidth, &textureHeight, &numberOfColorChannels, 0);
    if (!textureData) {
        PRINT("Texture::init(): failed to load texture");
        PRINTLN(path);

        return;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);

    initialized = true;
}

unsigned int Texture::getTextureId() const {
    if (!initialized) {
        PRINTLN("cant get textureId of uninitialized texture");
        return 0;
    }
    if (released) {
        PRINTLN("cant get textureId of released texture");
        return 0;
    }
    return textureId;
}

int Texture::getTextureWidth() const {
    if (!initialized) {
        PRINTLN("cant getWidth of uninitialized texture");
        return 0;
    }
    if (released) {
        PRINTLN("cant getWidth of released texture");
        return 0;
    }
    return textureWidth;
}

int Texture::getTextureHeight() const {
    if (!initialized) {
        PRINTLN("cant getHeight of uninitialized texture");
        return 0;
    }
    if (released) {
        PRINTLN("cant getHeight of released texture");
        return 0;
    }
    return textureHeight;
}

void Texture::release() {
    if (released) {
        return;
    }
    
    if (!initialized) {
        PRINTLN("cant release uninitialized texture");
        return;
    }

    glDeleteTextures(1, &textureId);
    
    released = true;
}
