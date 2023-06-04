#include <Codes/UI/ui.h>

#include <Codes/Types/vec2.h>
#include <Codes/Types/color.h>
#include <glad/glad.h>

#include <Codes/print.h>

extern int currentWindowWidth;
extern int currentWindowHeight;

Mesh UI::rectMesh;
Shader UI::rectShader;

Texture UI::crosshairTexture;

void UI::init() {
    std::vector<float> rectVerticies = {
        0, 1,
        1, 1,
        0, 0,
        
        1, 1,
        1, 0,
        0, 0,
    };
    rectMesh.set2d(rectVerticies);

    rectShader.init("Shaders/UI/rect");
    rectShader.useProgram();
    rectShader.setUniform("windowSize", Vec2((float)currentWindowWidth, (float)currentWindowHeight));



    crosshairTexture.init("Textures/UI/crosshair.png");
}

void UI::draw() {
    drawTexture(currentWindowWidth/2, currentWindowHeight/2, 
                crosshairTexture.getWidth(), crosshairTexture.getHeight(), 
                crosshairTexture, true);
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

    rectShader.useProgram();
    rectShader.setUniform("rectPos", Vec2(x1, y1));
    rectShader.setUniform("rectSize", Vec2(x2 - x1, y2 - y1));
    rectShader.setUniform("drawTexture", false);
    rectShader.setUniform("rectColor", color);

    rectMesh.draw();
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
        h = (float)texture.getHeight() / (float)texture.getWidth() * w;
    } else if (w == 0) {
        w = (float)texture.getWidth() / (float)texture.getHeight() * h;
    }

    y = currentWindowHeight - y;

    if (centered) {
        x -= w/2;
        y -= h/2;
    }

    rectShader.useProgram();
    rectShader.setUniform("rectPos", Vec2(x, y));
    rectShader.setUniform("rectSize", Vec2(w, h));
    rectShader.setUniform("drawTexture", true);
    rectShader.setUniform("rectTexture", texture, 0);

    rectMesh.draw();
}

void UI::release() {
    rectMesh.release();
    rectShader.release();

    crosshairTexture.release();
}
