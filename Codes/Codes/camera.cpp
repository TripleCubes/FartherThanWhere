#include <Codes/camera.h>

#include <Codes/print.h>

Camera::Camera(const ChunkLoader& chunkLoader): chunkLoader(chunkLoader) {}

void Camera::update() {
    blockRaycastResult = blockRaycast.cast(pos, dir, 10);
}

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

const BlockRaycast::Result &Camera::getBlockRaycastResult() const {
    return blockRaycastResult;
}
