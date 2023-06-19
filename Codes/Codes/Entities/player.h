#ifndef PLAYER_H
#define PLAYER_H

#include <Codes/Types/vec3.h>

#include <Codes/GraphicTypes/mesh.h>

class Settings;
class ChunkLoader;

class Player {
public:
    Player(const Settings &settings, const ChunkLoader& chunkLoader);

    Vec3 getPos() const;
    void update();
    void draw() const;

    void move(Vec3 dir);
    void jump();

    ~Player();
    
private:
    const Settings &settings;
    const ChunkLoader &chunkLoader;
    static Mesh mesh;

    Vec3 pos = Vec3(0, 0, 0);
    void setPos(Vec3 pos);



    enum class Axis {
        X, Y, Z,
    };
    float moveSpeed = 0.2;
    
    void moveAxis(float moveAmount, Axis axis);

    Vec3 internalVelocity;
    Vec3 externalVelocity;

    float internalVelocityCap = 0;
    float internalVelocityAddAmount = 0;
    float internalVelocitySlowDownAmount = 0;

    float externalVelocityCap = 0;
    float externalVelocitySlowDownAmount = 0;
    float gravityVelocityAddAmount = 0;
    float jumpVelocityAddAmount = 0;

    bool onGround = false;

    void slowDownVelocity(Vec3 &velocity, float velocitySlowDownAmount);
    void addVelocity(Vec3 &velocity, Vec3 velocityAddDir, float velocityAddAmount, float velocityCap);
    void addGravity();
};

#endif
