#include <Codes/UI/ui.h>

#include <Codes/Types/vec2.h>
#include <Codes/Types/color.h>

#include <Codes/Debug/print.h>

extern int currentWindowWidth;
extern int currentWindowHeight;

Mesh UI::mesh_rect;
Shader UI::shader_rect;

Texture UI::texture_crosshair;

void UI::init() {
    std::vector<float> rectVerticies = {
        0, 1,
        1, 1,
        0, 0,
        
        1, 1,
        1, 0,
        0, 0,
    };
    mesh_rect.init2d(rectVerticies);

    shader_rect.init("Shaders/UI/rect");
    shader_rect.useProgram();
    shader_rect.setUniform("windowSize", Vec2((float)currentWindowWidth, (float)currentWindowHeight));



    texture_crosshair.init("Textures/UI/crosshair.png");
}

void UI::draw() {
    drawTexture(currentWindowWidth/2, currentWindowHeight/2,
                texture_crosshair.getTextureWidth(), texture_crosshair.getTextureHeight(), 
                texture_crosshair, true);

    drawTextBox(10, 10, "This is a test ggggg", Color(1, 0.65, 0.57, 0.5));
}

void UI::drawRectPos(float x1, float y1, float x2, float y2, Color color) {
    if (x1 > x2) {
        float x3 = x1;
        x1 = x2;
        x2 = x3;
    }
    if (y1 > y2) {
        float y3 = y1;
        y1 = y2;
        y2 = y3;
    }

    float y3 = y1;
    y1 = currentWindowHeight - y2;
    y2 = currentWindowHeight - y3;

    shader_rect.useProgram();
    shader_rect.setUniform("rectPos", Vec2(x1, y1));
    shader_rect.setUniform("rectSize", Vec2(x2 - x1, y2 - y1));
    shader_rect.setUniform("drawMode", DRAWMODE_RECT);
    shader_rect.setUniform("drawColor", color);

    mesh_rect.draw();
}

void UI::drawRectWH(float x, float y, float w, float h, Color color) {
    drawRectPos(x, y, x + w, y + h, color);
}

void UI::drawTexture(float x, float y, float w, float h, const Texture &texture, bool centered) {
    drawTexture(x, y, w, h, texture.getTextureId(), texture.getTextureWidth(), texture.getTextureHeight(), centered);
}

void UI::drawTexture(float x, float y, float w, float h, 
                    unsigned int textureId, int textureWidth, int textureHeight, 
                    bool centered) {
    drawTexture(false, x, y, w, h, textureId, textureWidth, textureHeight, Color(0, 0, 0, 0), centered);
}

void UI::drawTexture(bool isTextTexture, float x, float y, float w, float h, 
                    unsigned int textureId, int textureWidth, int textureHeight, 
                    Color textColor, bool centered) {
    if (w < 0) {
        PRINTLN("UI::drawTexture(): w < 0");
        return;
    }
    if (h < 0) {
        PRINTLN("UI::drawTexture(): h < 0");
        return;
    }
    if (w == 0 && h == 0) {
        PRINTLN("UI::drawTexture(): w == 0 && h == 0");
        return;
    }
    
    if (h == 0) {
        h = (float)textureHeight / (float)textureWidth * w;
    } else if (w == 0) {
        w = (float)textureWidth / (float)textureHeight * h;
    }

    y = currentWindowHeight - y - h;

    if (centered) {
        x -= w/2;
        y += h/2;
    }

    shader_rect.useProgram();
    shader_rect.setUniform("rectPos", Vec2(x, y));
    shader_rect.setUniform("rectSize", Vec2(w, h));
    shader_rect.setUniform("drawMode", isTextTexture? DRAWMODE_TEXT : DRAWMODE_TEXTURE);
    if (isTextTexture) {
        shader_rect.setUniform("drawColor", textColor);
    }
    shader_rect.setUniform("rectTexture", textureId, 0);

    mesh_rect.draw();
}

Vec2 UI::getTextBoxSize(std::string &text) {
    Vec2 result;

    for (std::size_t i = 0; i < text.size(); i++)
    {
        result.x += Text::getCharacter(text[i]).advance;
    }
    result.y = FONT_HEIGHT;

    return result;
}

void UI::drawTextBox(float x, float y, std::string &text, Color color) {
    int cursorX = x;
    int cursorY = y;
    for (std::size_t i = 0; i < text.size(); i++)
    {
        Text::Character drawnCharacter = drawTextChar(cursorX, cursorY, text[i], color);
        cursorX += drawnCharacter.advance;
    }
}

void UI::drawTextBox(float x, float y, const char *text, Color color) {
    std::string string(text);
    drawTextBox(x, y, string, color);
}

Text::Character UI::drawTextChar(float x, float y, char characterCode, Color color) {
    Text::Character character = Text::getCharacter(characterCode);
    if (characterCode != 32) {
        drawTexture(true, x + character.bearingX, y + FONT_HEIGHT - character.bearingY, character.w, character.h, 
        character.textureId, character.w, character.h, color, false);
    }

    return character;
}

void UI::release() {
    mesh_rect.release();
    shader_rect.release();

    texture_crosshair.release();
}
