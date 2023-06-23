#include <Codes/view.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Codes/Chunk/chunkLoader.h>
#include <Codes/camera.h>
#include <Codes/settings.h>
#include <Codes/Entities/player.h>
#include <Codes/Types/vec3.h>
#include <Codes/Types/color.h>
#include <Codes/Raycast/blockRaycast.h>
#include <Codes/Textures/blockTextures.h>

#include <glad/glad.h>
#include <Codes/print.h>

//
#include <Codes/GraphicEffects/boxBlur.h>
#include <Codes/GraphicEffects/ssao.h>

extern int currentWindowWidth;
extern int currentWindowHeight;

namespace GlobalGraphics {
    extern Mesh mesh_windowRect;
    extern Shader shader_windowRect;
}

Shader View::shader_gBuffer;
Shader View::shader_gBufferLayers;
Shader View::shader_view;
Shader View::shader_boxFrame;
Mesh View::mesh_boxFrame;

View::View(const Settings &settings, const Camera &camera, const ChunkLoader &chunkLoader, const Player &player): 
settings(settings), camera(camera), chunkLoader(chunkLoader), player(player) {
    shader_gBuffer.init("Shaders/View/gBuffer");
    shader_gBufferLayers.init("Shaders/View/gBufferLayers");
    framebuffer_gBuffer.init(0, 0, 3);

    shader_view.init("Shaders/View/view");
    framebuffer_view.init();

    shader_boxFrame.init("Shaders/View/boxFrame");

    std::vector<float> blockSelectionVerticies = {
        0, 1, 0, // A 0
        1, 1, 0, // B 1
        1, 1, 1, // C 2
        0, 1, 1, // D 3
        0, 0, 0, // E 4
        1, 0, 0, // F 5
        1, 0, 1, // G 6
        0, 0, 1, // H 7
    };
    std::vector<unsigned int> blockSelectionIndicies = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        0, 4,
        1, 5,
        2, 6,
        3, 7,
        4, 5,
        5, 6,
        6, 7,
        7, 4
    };
    mesh_boxFrame.init3d(blockSelectionVerticies, blockSelectionIndicies, true);
}

void View::update() {}

void View::draw() const {
    glm::mat4 projectionMat = glm::perspective(glm::radians(70.0f), 
                                                (float)currentWindowWidth / (float)currentWindowHeight, 
                                                0.1f, 300.0f);
    glm::mat4 viewMat = glm::mat4(1.0f);
    if (!settings.isThirdPersonView()) {
        viewMat = glm::lookAt((camera.getPos()).toGlmVec3(),
                                (camera.getPos() + camera.getDir()).toGlmVec3(),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    } else {
        viewMat = glm::lookAt((camera.getPos() - (camera.getDir() * 10)).toGlmVec3(),
                                (camera.getPos()).toGlmVec3(),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    }

    shader_gBuffer.useProgram();
    shader_gBuffer.setUniform("projectionMat", projectionMat);
    shader_gBuffer.setUniform("viewMat", viewMat);

    shader_gBufferLayers.useProgram();
    shader_gBufferLayers.setUniform("projectionMat", projectionMat);
    shader_gBufferLayers.setUniform("viewMat", viewMat);

    shader_boxFrame.useProgram();
    shader_boxFrame.setUniform("projectionMat", projectionMat);
    shader_boxFrame.setUniform("viewMat", viewMat);

    framebuffer_gBuffer.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    drawChunks();
    drawPlayer();

    if (settings.isSSAOEnabled()) {
        GraphicEffects::SSAO::createTexture(framebuffer_gBuffer.getTextureId(GBUFFER_POS),
                                            framebuffer_gBuffer.getTextureId(GBUFFER_NORMAL),
                                            projectionMat,
                                            viewMat);
        GraphicEffects::BoxBlur::createTexture(GraphicEffects::SSAO::getTextureId(), 5, 3);
    }

    framebuffer_view.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    shader_view.useProgram();
    shader_view.setUniform("gBuffer_pos", framebuffer_gBuffer.getTextureId(GBUFFER_POS), 0);
    shader_view.setUniform("gBuffer_normal", framebuffer_gBuffer.getTextureId(GBUFFER_NORMAL), 1);
    shader_view.setUniform("gBuffer_color", framebuffer_gBuffer.getTextureId(GBUFFER_COLOR), 2);
    if (settings.isSSAOEnabled()) {
        shader_view.setUniform("ssaoEnabled", true);
        shader_view.setUniform("texture_ssao", GraphicEffects::BoxBlur::getTextureId(), 3);
    } else {
        shader_view.setUniform("ssaoEnabled", false);
    }
    GlobalGraphics::mesh_windowRect.draw();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer_gBuffer.getFBO());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer_view.getFBO());
    glBlitFramebuffer(0, 0, currentWindowWidth, currentWindowHeight, 
                        0, 0, currentWindowWidth, currentWindowHeight,
                        GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    framebuffer_view.bind();
    glEnable(GL_DEPTH_TEST);
    drawChunkInformations();
    drawBlockSelection();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    GlobalGraphics::shader_windowRect.useProgram();
    GlobalGraphics::shader_windowRect.setUniform("texture", framebuffer_view.getTextureId(), 0);
    GlobalGraphics::mesh_windowRect.draw();
}

void View::drawChunks() const {
    if (settings.isWireframeMode())
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    shader_gBufferLayers.useProgram();
    shader_gBufferLayers.setUniform("texture", GameTextures::BlockTextures::getBlocksTextureId(), 0, true);

    for (const auto &i: chunkLoader.getChunkList()) {
        Vec3 pos = Vec3(i.first) * CHUNK_WIDTH;
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, pos.toGlmVec3());
        shader_gBufferLayers.setUniform("modelMat", modelMat);
        i.second->draw();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void View::drawChunkInformations() const {
    if (!settings.isShowingChunkInformations()) {
        return;
    }

    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, Vec3(IntPos(player.getPos()).getChunkPos() * CHUNK_WIDTH).toGlmVec3());
    shader_boxFrame.useProgram();
    shader_boxFrame.setUniform("modelMat", modelMat);
    shader_boxFrame.setUniform("boxSize", Vec3(CHUNK_WIDTH, CHUNK_WIDTH, CHUNK_WIDTH));
    shader_boxFrame.setUniform("boxMargin", Vec3(0, 0, 0));
    shader_boxFrame.setUniform("frameColor", Color(0.8, 1, 0.92, 1));

    glLineWidth(2);
    mesh_boxFrame.draw();
    glLineWidth(1);
}

void View::drawBlockSelection() const {
    if (settings.isThirdPersonView() || !camera.getBlockRaycastResult().found) {
        return;
    }
    
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, Vec3(camera.getBlockRaycastResult().selectedPos).toGlmVec3());
    shader_boxFrame.useProgram();
    shader_boxFrame.setUniform("modelMat", modelMat);
    shader_boxFrame.setUniform("boxSize", Vec3(1, 1, 1));
    shader_boxFrame.setUniform("boxMargin", Vec3(0, 0, 0));
    if (settings.isWireframeMode()) {
        shader_boxFrame.setUniform("frameColor", Color(1, 1, 1, 1));
    } else {
        shader_boxFrame.setUniform("frameColor", Color(0, 0, 0, 1));
    }

    glLineWidth(2);
    mesh_boxFrame.draw();
    glLineWidth(1);
}

void View::drawPlayer() const {
    if (!settings.isThirdPersonView()) {
        return;
    }

    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, player.getPos().toGlmVec3());
    shader_gBuffer.useProgram();
    shader_gBuffer.setUniform("modelMat", modelMat);

    player.draw();
}

View::~View() {
    shader_gBuffer.release();
    shader_gBufferLayers.release();
    framebuffer_gBuffer.release();

    shader_view.release();
    framebuffer_view.release();

    shader_boxFrame.release();
    mesh_boxFrame.release();
}
