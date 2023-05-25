#ifndef VIEW_H
#define VIEW_H

#include <Codes/Types/vec3.h>
#include <Codes/GraphicTypes/shader.h>
class ChunkLoader;
class Settings;

class View {
    friend class Controls;

public:
    View(const Settings &settings, const ChunkLoader &chunkLoader);
    
    void update();

    void draw() const;

    Vec3 getCameraPos() const;
    Vec3 getCameraDir() const;

    ~View();
    
private:
    Vec3 cameraPos = Vec3(0, 0, 0);
    Vec3 cameraDir = Vec3(0, 0, -1);
    Shader shader;

    const Settings &settings;
    const ChunkLoader &chunkLoader;

    void setCameraPos(Vec3 cameraPos);
    void setCameraDir(Vec3 cameraDir);
};

#endif
