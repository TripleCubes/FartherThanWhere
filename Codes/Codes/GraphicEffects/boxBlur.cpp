#include <Codes/GraphicEffects/boxBlur.h>

#include <Codes/GraphicTypes/texture.h>
#include <glad/glad.h>

namespace GlobalGraphics {
    extern Mesh mesh_windowRect;
}

Shader GraphicEffects::BoxBlur::shader_blur;
Framebuffer GraphicEffects::BoxBlur::framebuffer_horizontalBlurred;
Framebuffer GraphicEffects::BoxBlur::framebuffer_blurred;

void GraphicEffects::BoxBlur::init() {
    shader_blur.init("Shaders/GraphicEffects/boxBlur");

    framebuffer_horizontalBlurred.init();
    framebuffer_blurred.init();
}

void GraphicEffects::BoxBlur::createTexture(Texture inputTexture, int blurPixelRange, int blurTimes) {
    createTexture(inputTexture.getTextureId(), blurPixelRange, blurTimes);
}

void GraphicEffects::BoxBlur::createTexture(unsigned int inputTextureId, int blurPixelRange, int blurTimes) {
    shader_blur.useProgram();
    shader_blur.setUniform("blurSize", blurPixelRange);
    createBlurTexture1Time(inputTextureId);
    for (int i = 0; i < blurTimes - 1; i++)
    {
        createBlurTexture1Time(framebuffer_blurred.getTextureId());
    }
}

void GraphicEffects::BoxBlur::createBlurTexture1Time(unsigned int inputTextureId) {
    framebuffer_horizontalBlurred.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    shader_blur.useProgram();
    shader_blur.setUniform("horizontal", true);
    shader_blur.setUniform("texture", inputTextureId, 0);
    GlobalGraphics::mesh_windowRect.draw();

    framebuffer_blurred.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    shader_blur.setUniform("horizontal", false);
    shader_blur.setUniform("texture", framebuffer_horizontalBlurred.getTextureId(), 0);
    GlobalGraphics::mesh_windowRect.draw();
}

unsigned int GraphicEffects::BoxBlur::getTextureId() {
    return framebuffer_blurred.getTextureId();
}

void GraphicEffects::BoxBlur::release() {
    shader_blur.release();
    framebuffer_horizontalBlurred.release();
    framebuffer_blurred.release();
}
