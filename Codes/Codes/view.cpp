#include <Codes/view.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Codes/Chunk/chunkLoader.h>
#include <Codes/settings.h>

#include <glad/glad.h>
#include <Codes/print.h>

extern int currentWindowWidth;
extern int currentWindowHeight;

View::View(const Settings &settings, const ChunkLoader &chunkLoader): 
settings(settings), chunkLoader(chunkLoader) {
    shader.init("Shaders/view");
    cameraPos = Vec3(0, 30, 0);
}

void View::update() {}

void View::setCameraPos(Vec3 cameraPos) {
    View::cameraPos = cameraPos;
}

void View::setCameraDir(Vec3 cameraDir) {
    View::cameraDir = cameraDir;
}

void View::draw() const {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (settings.isWireframeMode())
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glm::mat4 projectionMat = glm::perspective(glm::radians(70.0f), 
                                                (float)currentWindowWidth / (float)currentWindowHeight, 
                                                0.1f, 300.0f);
    glm::mat4 viewMat = glm::mat4(1.0f);
    if (!settings.isThirdPersonView()) {
        viewMat = glm::lookAt((cameraPos + Vec3(0, 1.5, 0)).toGlmVec3(),
                                (cameraPos + Vec3(0, 1.5, 0) + cameraDir).toGlmVec3(),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    } else {
        viewMat = glm::lookAt((cameraPos + Vec3(0, 1.5, 0) - (cameraDir * 10)).toGlmVec3(),
                                (cameraPos + Vec3(0, 1.5, 0)).toGlmVec3(),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    }

    shader.useProgram();
    shader.setUniform("projectionMat", projectionMat);
    shader.setUniform("viewMat", viewMat);

    for (const auto &i: chunkLoader.chunks) {
        Vec3 pos = Vec3(i.first) * CHUNK_WIDTH;
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, pos.toGlmVec3());
        shader.setUniform("modelMat", modelMat);
        i.second->draw();
    }
}

Vec3 View::getCameraPos() const {
    return cameraPos;
}

Vec3 View::getCameraDir() const {
    return cameraDir;
}

View::~View() {
    shader.release();
}
