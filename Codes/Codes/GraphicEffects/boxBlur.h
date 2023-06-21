#ifndef BOXBLUR_H
#define BOXBLUR_H

#include <Codes/GraphicTypes/framebuffer.h>
#include <Codes/GraphicTypes/shader.h>
#include <Codes/GraphicTypes/mesh.h>
class Texture;

namespace GraphicEffects {
    class BoxBlur {
    public:
        static void init();

        static void createTexture(unsigned int inputTextureId, int blurPixelRange, int blurTimes);
        static void createTexture(Texture inputTexture, int blurPixelRange, int blurTimes);
        static unsigned int getTextureId();

        static void release();

    private:
        static Shader shader_blur;
        static Framebuffer framebuffer_horizontalBlurred;
        static Framebuffer framebuffer_blurred;

        static void createBlurTexture1Time(unsigned int inputtextureId);
    };
}

#endif
