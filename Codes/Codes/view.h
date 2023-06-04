#ifndef VIEW_H
#define VIEW_H

#include <Codes/GraphicTypes/shader.h>
#include <Codes/camera.h>
class ChunkLoader;
class Settings;
class Vec3;

class View {
    friend class Controls;

public:
    View(const Settings &settings, const ChunkLoader &chunkLoader);
    
    void update();

    void draw() const;

    ~View();
    
private:
    const Settings &settings;
    const ChunkLoader &chunkLoader;

    Camera camera = Camera(chunkLoader);
    Shader shader;
};

#endif
