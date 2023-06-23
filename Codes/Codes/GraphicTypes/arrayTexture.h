#ifndef ARRAYTEXTURE_H
#define ARRAYTEXTURE_H

#include <Codes/GraphicTypes/texture.h>

class ArrayTexture: public Texture {
public:
    void init(const std::string &path) = delete;
    void init(const std::string &path, int layerWidth, int layerHeight);

private:
    int layerWidth = 0;
    int layerHeight = 0;
    int layerCount = 0;

    int getLayerWidth() const;
    int getLayerHeight() const;
    int getLayerCount() const;
};

#endif
