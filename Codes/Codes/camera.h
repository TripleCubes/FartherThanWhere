#ifndef CAMERA_H
#define CAMERA_H

#include <Codes/Types/vec3.h>
#include <Codes/Raycast/blockRaycast.h>
class ChunkLoader;

class Camera {
public:
    Camera(const ChunkLoader &chunkLoader);

    void update();

    Vec3 getPos() const;
    void setPos(Vec3 pos);
    Vec3 getDir() const;
    void setDir(Vec3 dir);
    const BlockRaycast::Result &getBlockRaycastResult() const;

private:
    Vec3 pos = Vec3(0, 0, 0);
    Vec3 dir = Vec3(0, 0, 1);

    const ChunkLoader &chunkLoader;

    BlockRaycast blockRaycast = BlockRaycast(chunkLoader);
    BlockRaycast::Result blockRaycastResult;
};

#endif
