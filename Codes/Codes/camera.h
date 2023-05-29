#ifndef CAMERA_H
#define CAMERA_H

#include <Codes/Types/vec3.h>

class Camera {
public:
    Vec3 getPos() const;
    void setPos(Vec3 pos);
    Vec3 getDir() const;
    void setDir(Vec3 dir);

private:
    Vec3 pos = Vec3(0, 0, 0);
    Vec3 dir = Vec3(0, 0, 1);
};

#endif
