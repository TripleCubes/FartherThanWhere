#ifndef PLAYER_H
#define PLAYER_H

#include <Codes/Types/vec3.h>

#include <Codes/GraphicTypes/mesh.h>

class Settings;
class ChunkLoader;

class Player {
    friend class Controls;

public:
    Player(const Settings &settings, const ChunkLoader& chunkLoader);

    Vec3 getPos() const;
    void draw() const;

    ~Player();
    
private:
    const Settings &settings;
    const ChunkLoader &chunkLoader;
    Mesh mesh;

    Vec3 pos = Vec3(0, 0, 0);
    void setPos(Vec3 pos);

    enum class Axis {
        X, Y, Z,
    };
    float moveSpeed = 0.2;
    void move(Vec3 dir);
    void moveAxis(float moveAmount, Axis axis);
};

#endif
