#include <Codes/GraphicEffects/ssao.h>

#include <Codes/random.h>
#include <Codes/GraphicTypes/mesh.h>
#include <glad/glad.h>
#include <cmath>

namespace GlobalGraphics {
    extern Mesh mesh_windowRect;
}

const float pi = 4*atan(1);

Framebuffer GraphicEffects::SSAO::framebuffer;
Shader GraphicEffects::SSAO::shader_ssao;
std::vector<Vec3> GraphicEffects::SSAO::randomPointList;
std::vector<Vec3> GraphicEffects::SSAO::randomRotationVecList;

void GraphicEffects::SSAO::init() {
    shader_ssao.init("Shaders/GraphicEffects/ssao");
    framebuffer.init();

    createRandomPointList(20);
    createRandomRotationVecList(10);
}

void GraphicEffects::SSAO::createRandomPointList(int numberOfPoints) {
    randomPointList.clear();

    for (int i = 0; i < numberOfPoints; i++) {
        Vec3 point = Vec3(Random::getFloat(-1, 1), 
                            Random::getFloat(-1, 1), 
                            Random::getFloat(-1, 1)).normalize() * Random::getFloat(0, 1);
        randomPointList.push_back(point);
    }
}

void GraphicEffects::SSAO::createRandomRotationVecList(int numberOfRotationVecs) {
    randomRotationVecList.clear();

    for (int i = 0; i < numberOfRotationVecs; i++) {
        randomRotationVecList.push_back(Vec3(Random::getFloat(-pi, pi), 
                                            Random::getFloat(-pi, pi), 
                                            Random::getFloat(-pi, pi)));
    }
}

void GraphicEffects::SSAO::createTexture(unsigned int gBuffer_pos_textureId, 
                                            unsigned int gBuffer_normal_textureId,
                                            glm::mat4 &projectionMat, glm::mat4 &viewMat) {
    framebuffer.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    shader_ssao.useProgram();
    shader_ssao.setUniform("gBuffer_pos", gBuffer_pos_textureId, 0);
    shader_ssao.setUniform("gBuffer_normal", gBuffer_normal_textureId, 1);
    shader_ssao.setUniform("projectionMat", projectionMat);
    shader_ssao.setUniform("viewMat", viewMat);

    shader_ssao.setUniform("numberOfRandomPoints", (int)randomPointList.size());
    for (std::size_t i = 0; i < randomPointList.size(); i++) {
        shader_ssao.setUniform("randomPoints[" + std::to_string(i) + "]", randomPointList[i]);
    }
    shader_ssao.setUniform("numberOfRandomRotationVecs", (int)randomRotationVecList.size());
    for (std::size_t i = 0; i < randomRotationVecList.size(); i++) {
        shader_ssao.setUniform("randomRotationVecs[" + std::to_string(i) + "]", randomRotationVecList[i]);
    }

    GlobalGraphics::mesh_windowRect.draw();
}

unsigned int GraphicEffects::SSAO::getTextureId() {
    return framebuffer.getTextureId();
}

void GraphicEffects::SSAO::release() {
    framebuffer.release();
    shader_ssao.release();
}
