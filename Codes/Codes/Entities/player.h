#ifndef PLAYER_H
#define PLAYER_H

#include <Codes/Types/vec3.h>

class Player {
public:
    void setPos(Vec3 pos);
    Vec3 getPos() const;
    
private:
    Vec3 pos = Vec3(0, 30, 0);
};

#endif
