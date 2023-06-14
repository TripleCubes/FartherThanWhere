#include <Codes/GraphicEffects/boxBlur.h>

#include <Codes/GraphicTypes/texture.h>
#include <glad/glad.h>

Shader GraphicEffects::BoxBlur::shader_blur;
Mesh GraphicEffects::BoxBlur::mesh_windowRect;
Framebuffer GraphicEffects::BoxBlur::framebuffer_horizontalBlurred;
Framebuffer GraphicEffects::BoxBlur::framebuffer_blurred;

void GraphicEffects::BoxBlur::init() {
    shader_blur.init("Shaders/Effects/boxBlur");

    std::vector<float> windowRectVerticies = {
        -1,  1,
         1,  1,
        -1, -1,
        
         1,  1,
         1, -1,
        -1, -1,
    };
    mesh_windowRect.set2d(windowRectVerticies);

    framebuffer_horizontalBlurred.init();
    framebuffer_blurred.init();
}

void GraphicEffects::BoxBlur::createBlurTexture(Texture inputTexture, int blurPixelRange, int blurTimes) {
    createBlurTexture(inputTexture.getTextureId(), blurPixelRange, blurTimes);
}

void GraphicEffects::BoxBlur::createBlurTexture(unsigned int inputTextureId, int blurPixelRange, int blurTimes) {
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader_blur.useProgram();
    shader_blur.setUniform("horizontal", true);
    shader_blur.setUniform("texture", inputTextureId, 0);
    mesh_windowRect.draw();

    framebuffer_blurred.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader_blur.setUniform("horizontal", false);
    shader_blur.setUniform("texture", framebuffer_horizontalBlurred.getTextureId(), 0);
    mesh_windowRect.draw();
}

unsigned int GraphicEffects::BoxBlur::getBlurredTexture() {
    return framebuffer_blurred.getTextureId();
}

void GraphicEffects::BoxBlur::release() {
    shader_blur.release();
    mesh_windowRect.release();
    framebuffer_horizontalBlurred.release();
    framebuffer_blurred.release();
}
