#include <Codes/UI/ui.h>

#include <Codes/Types/vec2.h>
#include <Codes/Types/color.h>

#include <Codes/print.h>

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
    shader_rect.setUniform("drawTexture", false);
    shader_rect.setUniform("rectColor", color);

    mesh_rect.draw();
}

void UI::drawRectWH(float x, float y, float w, float h, Color color) {
    drawRectPos(x, y, x + w, y + h, color);
}

void UI::drawTexture(float x, float y, float w, float h, const Texture &texture, bool centered) {
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
        h = (float)texture.getTextureHeight() / (float)texture.getTextureWidth() * w;
    } else if (w == 0) {
        w = (float)texture.getTextureWidth() / (float)texture.getTextureHeight() * h;
    }

    y = currentWindowHeight - y;

    if (centered) {
        x -= w/2;
        y -= h/2;
    }

    shader_rect.useProgram();
    shader_rect.setUniform("rectPos", Vec2(x, y));
    shader_rect.setUniform("rectSize", Vec2(w, h));
    shader_rect.setUniform("drawTexture", true);
    shader_rect.setUniform("rectTexture", texture, 0);

    mesh_rect.draw();
}

void UI::release() {
    mesh_rect.release();
    shader_rect.release();

    texture_crosshair.release();
}
