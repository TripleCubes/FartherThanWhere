#include <Codes/camera.h>

Vec3 Camera::getPos() const {
    return pos;
}

void Camera::setPos(Vec3 pos) {
    this->pos = pos;
}

Vec3 Camera::getDir() const {
    return dir;
}

void Camera::setDir(Vec3 dir) {
    this->dir = dir;
}
