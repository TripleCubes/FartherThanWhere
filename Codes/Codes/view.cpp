#include <Codes/view.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Codes/Chunk/chunkLoader.h>
#include <Codes/camera.h>
#include <Codes/settings.h>
#include <Codes/Entities/player.h>
#include <Codes/Types/vec3.h>
#include <Codes/Raycast/blockRaycast.h>

#include <glad/glad.h>
#include <Codes/print.h>

extern int currentWindowWidth;
extern int currentWindowHeight;

View::View(const Settings &settings, const Camera &camera, const ChunkLoader &chunkLoader, const Player &player): 
settings(settings), camera(camera), chunkLoader(chunkLoader), player(player) {
    viewShader.init("Shaders/View/view");

    blockSelectionShader.init("Shaders/View/blockSelection");

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
    blockSelectionMesh.set3d(blockSelectionVerticies, blockSelectionIndicies, true);
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

    viewShader.useProgram();
    viewShader.setUniform("projectionMat", projectionMat);
    viewShader.setUniform("viewMat", viewMat);

    blockSelectionShader.useProgram();
    blockSelectionShader.setUniform("projectionMat", projectionMat);
    blockSelectionShader.setUniform("viewMat", viewMat);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    drawChunks(projectionMat, viewMat);
    drawBlockSelection(projectionMat, viewMat);
    drawPlayer();
}

void View::drawChunks(glm::mat4 &projectionMat, glm::mat4 &viewMat) const {
    if (settings.isWireframeMode())
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    viewShader.useProgram();

    for (const auto &i: chunkLoader.getChunkList()) {
        Vec3 pos = Vec3(i.first) * CHUNK_WIDTH;
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, pos.toGlmVec3());
        viewShader.setUniform("modelMat", modelMat);
        i.second->draw();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void View::drawBlockSelection(glm::mat4 &projectionMat, glm::mat4 &viewMat) const {
    if (settings.isThirdPersonView() || !camera.getBlockRaycastResult().found) {
        return;
    }
    
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, Vec3(camera.getBlockRaycastResult().selectedPos).toGlmVec3());
    blockSelectionShader.useProgram();
    blockSelectionShader.setUniform("modelMat", modelMat);

    glLineWidth(2);
    blockSelectionMesh.draw();
    glLineWidth(1);
}

void View::drawPlayer() const {
    if (!settings.isThirdPersonView()) {
        return;
    }

    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, player.getPos().toGlmVec3());
    viewShader.useProgram();
    viewShader.setUniform("modelMat", modelMat);

    player.draw();
}

View::~View() {
    viewShader.release();

    blockSelectionShader.release();
    blockSelectionMesh.release();
}
