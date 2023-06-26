#include <Codes/GraphicTypes/arrayTexture.h>

#include <glad/glad.h>
#include <stb_image.h>
#include <Codes/Debug/print.h>

void ArrayTexture::init(const std::string &path, int layerWidth, int layerHeight) {
    if (initialized) {
        return;
    }

    unsigned char *textureData = stbi_load(path.c_str(), &textureWidth, &textureHeight, &numberOfColorChannels, 0);
    if (!textureData) {
        PRINT("ArrayTexture::init(): failed to load texture");
        PRINTLN(path);

        return;
    }

    this->layerWidth = layerWidth;
    this->layerHeight = layerHeight;
    int layerCountX = textureWidth / layerWidth;
    int layerCountY = textureHeight / layerHeight;
    this->layerCount = layerCountX * layerCountY;
    if (layerCount > 256) {
        PRINTLN("ArrayTexture::init(): layerCount > 256");
        return;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, layerWidth, layerHeight, layerCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    for (int layerIndex = 0; layerIndex < layerCount; layerIndex++) {
        unsigned char layerData[8*8*4];
        int layerIndexX = layerIndex % layerCountX;
        int layerIndexY = layerIndex / layerCountX;
        int layerDataPixelIndex = 0;
        for (int textureY = layerIndexY*layerHeight; textureY < (layerIndexY+1)*layerHeight; textureY++) {
            for (int textureX = layerIndexX*layerWidth; textureX < (layerIndexX+1)*layerWidth; textureX++) {
                for (int textureRGBAIndex = 0; textureRGBAIndex < 4; textureRGBAIndex++) {
                    layerData[layerDataPixelIndex] = textureData[textureY*textureWidth*4 + textureX*4 + textureRGBAIndex];
                    layerDataPixelIndex++;
                }
            }
        }
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layerIndex, layerWidth, layerHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, layerData);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    stbi_image_free(textureData);

    initialized = true;
}

int ArrayTexture::getLayerWidth() const {
    if (!initialized) {
        PRINTLN("cant get layerWidth of uninitialized texture");
        return 0;
    }
    if (released) {
        PRINTLN("cant get layerWidth of released texture");
        return 0;
    }

    return layerWidth;
}

int ArrayTexture::getLayerHeight() const {
    if (!initialized) {
        PRINTLN("cant get layerHeight of uninitialized texture");
        return 0;
    }
    if (released) {
        PRINTLN("cant get layerHeight of released texture");
        return 0;
    }

    return layerHeight;
}

int ArrayTexture::getLayerCount() const {
    if (!initialized) {
        PRINTLN("cant get layerCount of uninitialized texture");
        return 0;
    }
    if (released) {
        PRINTLN("cant get layerCount of released texture");
        return 0;
    }

    return layerCount;
}
