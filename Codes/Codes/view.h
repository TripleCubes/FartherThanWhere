#ifndef VIEW_H
#define VIEW_H

#include <Codes/GraphicTypes/shader.h>
#include <Codes/GraphicTypes/mesh.h>
#include <Codes/GraphicTypes/framebuffer.h>
#include <Codes/camera.h>
#include <glm/glm.hpp>
class ChunkLoader;
class Settings;
class Vec3;
class Player;
class Camera;

class View {
public:
    View(const Settings &settings, const Camera &camera, const ChunkLoader &chunkLoader, const Player &player);
    
    void update();

    void draw() const;

    ~View();
    
private:
    const Settings &settings;
    const Camera &camera;
    const ChunkLoader &chunkLoader;
    const Player &player;

    Shader shader_view;
    Framebuffer framebuffer_view;
    
    Mesh mesh_windowRect;
    Shader shader_windowRect;

    void drawChunks() const;

    void drawChunkInformations() const;

    Shader shader_boxFrame;
    Mesh mesh_boxFrame;
    void drawBlockSelection() const;

    void drawPlayer() const;
};

#endif
