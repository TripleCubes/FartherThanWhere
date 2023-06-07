#ifndef PLAYER_H
#define PLAYER_H

#include <Codes/Types/vec3.h>

#include <Codes/GraphicTypes/mesh.h>

class Player {
    friend class Controls;

public:
    Player();

    Vec3 getPos() const;
    void draw() const;

    ~Player();
    
private:
    Vec3 pos = Vec3(0, 0, 0);
    void setPos(Vec3 pos);

    Mesh mesh;
};

#endif
