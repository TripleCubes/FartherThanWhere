#include <Codes/view.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Codes/Chunk/chunkLoader.h>
#include <Codes/settings.h>
#include <Codes/Types/vec3.h>

#include <glad/glad.h>
#include <Codes/print.h>

extern int currentWindowWidth;
extern int currentWindowHeight;

View::View(const Settings &settings, const ChunkLoader &chunkLoader): 
settings(settings), chunkLoader(chunkLoader) {
    shader.init("Shaders/view");
    camera.setPos(Vec3(0, 30, 0));
}

void View::update() {
    camera.update();
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
        viewMat = glm::lookAt((camera.getPos()).toGlmVec3(),
                                (camera.getPos() + camera.getDir()).toGlmVec3(),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    } else {
        viewMat = glm::lookAt((camera.getPos() - (camera.getDir() * 10)).toGlmVec3(),
                                (camera.getPos()).toGlmVec3(),
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

View::~View() {
    shader.release();
}
