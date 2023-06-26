#ifndef UI_H
#define UI_h

#include <Codes/GraphicTypes/texture.h>
#include <Codes/GraphicTypes/mesh.h>
#include <Codes/GraphicTypes/shader.h>
#include <Codes/Types/vec2.h>
#include <Codes/Graphics/text.h>
#include <string>

class Color;

class UI {
public:
    static void init();

    static void draw();

    static void drawRectPos(float x1, float y1, float x2, float y2, Color color);
    static void drawRectWH(float x, float y, float w, float h, Color color);
    static void drawTexture(float x, float y, float w, float h, const Texture &texture, bool centered = false);
    static void drawTexture(float x, float y, float w, float h, 
                            unsigned int textureId, int textureWidth, int textureHeight, 
                            bool centered = false);

    static Vec2 getTextBoxSize(std::string &text);
    static void drawTextBox(float x, float y, std::string &text, Color color);
    static void drawTextBox(float x, float y, const char *text, Color color);

    static void release();

private:
    static const int DRAWMODE_RECT = 0;
    static const int DRAWMODE_TEXTURE = 1;
    static const int DRAWMODE_TEXT = 2;

    static Mesh mesh_rect;
    static Shader shader_rect;

    static Texture texture_crosshair;

    static Text::Character drawTextChar(float x, float y, char characterCode, Color color);
    static void drawTexture(bool isTextTexture, float x, float y, float w, float h, 
                            unsigned int textureId, int textureWidth, int textureHeight, 
                            Color textColor, bool centered);
};

#endif
