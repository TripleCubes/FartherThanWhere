#include <Codes/view.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Codes/Chunk/chunkLoader.h>
#include <Codes/camera.h>
#include <Codes/settings.h>
#include <Codes/Entities/player.h>
#include <Codes/Types/vec3.h>
#include <Codes/Types/color.h>
#include <Codes/Raycast/blockRaycast.h>

#include <glad/glad.h>
#include <Codes/print.h>

//
#include <Codes/GraphicEffects/boxBlur.h>

extern int currentWindowWidth;
extern int currentWindowHeight;

namespace GlobalGraphics {
    extern Mesh mesh_windowRect;
    extern Shader shader_windowRect;
}

Shader View::shader_view;
Shader View::shader_boxFrame;
Mesh View::mesh_boxFrame;

View::View(const Settings &settings, const Camera &camera, const ChunkLoader &chunkLoader, const Player &player): 
settings(settings), camera(camera), chunkLoader(chunkLoader), player(player) {
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

    shader_view.useProgram();
    shader_view.setUniform("projectionMat", projectionMat);
    shader_view.setUniform("viewMat", viewMat);

    shader_boxFrame.useProgram();
    shader_boxFrame.setUniform("projectionMat", projectionMat);
    shader_boxFrame.setUniform("viewMat", viewMat);

    framebuffer_view.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    drawChunks();
    drawChunkInformations();
    drawBlockSelection();
    drawPlayer();

    // GraphicEffects::BoxBlur::createBlurTexture(framebuffer_view.getTextureId(), 5, 2);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    GlobalGraphics::shader_windowRect.useProgram();
    GlobalGraphics::shader_windowRect.setUniform("texture", framebuffer_view.getTextureId(), 0);
    GlobalGraphics::mesh_windowRect.draw();
}

void View::drawChunks() const {
    if (settings.isWireframeMode())
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    shader_view.useProgram();

    for (const auto &i: chunkLoader.getChunkList()) {
        Vec3 pos = Vec3(i.first) * CHUNK_WIDTH;
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, pos.toGlmVec3());
        shader_view.setUniform("modelMat", modelMat);
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
    shader_boxFrame.setUniform("frameColor", Color(0, 0, 0, 1));

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
    shader_view.useProgram();
    shader_view.setUniform("modelMat", modelMat);

    player.draw();
}

View::~View() {
    shader_view.release();
    framebuffer_view.release();

    shader_boxFrame.release();
    mesh_boxFrame.release();
}
