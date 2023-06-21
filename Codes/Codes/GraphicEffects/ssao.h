#ifndef SSAO_H
#define SSAO_H

#include <Codes/GraphicTypes/framebuffer.h>
#include <Codes/GraphicTypes/texture.h>
#include <Codes/GraphicTypes/shader.h>
#include <Codes/Types/vec3.h>
#include <glm/glm.hpp>
#include <vector>

namespace GraphicEffects {
    class SSAO {
    public:
        static void init();

        static void createRandomPointList(int numberOfPoints);
        static void createRandomRotationVecList(int numberOfRotationVecs);
        static void createTexture(unsigned int gBuffer_pos_textureId, 
                                    unsigned int gBuffer_normal_textureId,
                                    glm::mat4 &projectionMat, glm::mat4 &viewMat);
        static unsigned int getTextureId();

        static void release();

    private:
        static Framebuffer framebuffer;
        static Shader shader_ssao;

        static std::vector<Vec3> randomPointList;
        static std::vector<Vec3> randomRotationVecList;
    };
}

#endif
