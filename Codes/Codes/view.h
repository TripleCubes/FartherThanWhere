#ifndef VIEW_H
#define VIEW_H

#include <Codes/GraphicTypes/shader.h>
#include <Codes/GraphicTypes/mesh.h>
#include <Codes/camera.h>
#include <glm/glm.hpp>
class ChunkLoader;
class Settings;
class Vec3;
class Player;

class View {
    friend class Controls;

public:
    View(const Settings &settings, const ChunkLoader &chunkLoader, const Player &player);
    
    void update();

    void draw() const;

    ~View();
    
private:
    const Settings &settings;
    const ChunkLoader &chunkLoader;
    const Player &player;

    Camera camera = Camera(chunkLoader, player);
    Shader viewShader;

    void drawChunks(glm::mat4 &projectionMat, glm::mat4 &viewMat) const;

    Shader blockSelectionShader;
    Mesh blockSelectionMesh;
    void drawBlockSelection(glm::mat4 &projectionMat, glm::mat4 &viewMat) const;

    void drawPlayer() const;
};

#endif
