#ifndef UI_H
#define UI_h

#include <Codes/GraphicTypes/texture.h>
#include <Codes/GraphicTypes/mesh.h>
#include <Codes/GraphicTypes/shader.h>

#include <Codes/Types/color.h>

class UI {
public:
    static void init();

    static void draw();

    static void release();

private:
    static Mesh rectMesh;
    static Shader rectShader;

    static Texture crosshairTexture;

    static void drawRectPos(float x1, float y1, float x2, float y2, Color color);
    static void drawRectWH(float x, float y, float w, float h, Color color);
    static void drawTexture(float x, float y, float w, float h, const Texture &texture, bool centered = false);
};

#endif
