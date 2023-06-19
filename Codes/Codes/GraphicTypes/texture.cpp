#include <Codes/GraphicTypes/texture.h>

#include <glad/glad.h>
#include <stb_image.h>
#include <Codes/print.h>

Texture::Texture() {}

void Texture::init(const std::string &path) {
    if (initialized) {
        return;
    }

    unsigned char *textureData = stbi_load(path.c_str(), &width, &height, &numberOfColorChannels, 0);
    if (!textureData) {
        PRINT("failed to load texture");
        PRINTLN(path);

        return;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

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

int Texture::getWidth() const {
    if (!initialized) {
        PRINTLN("cant getWidth of uninitialized texture");
        return 0;
    }
    if (released) {
        PRINTLN("cant getWidth of released texture");
        return 0;
    }
    return width;
}

int Texture::getHeight() const {
    if (!initialized) {
        PRINTLN("cant getHeight of uninitialized texture");
        return 0;
    }
    if (released) {
        PRINTLN("cant getHeight of released texture");
        return 0;
    }
    return height;
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
