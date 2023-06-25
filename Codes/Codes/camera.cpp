#include <Codes/camera.h>

#include <Codes/Entities/player.h>
#include <Codes/Debug/print.h>

Camera::Camera(const ChunkLoader &chunkLoader, const Player &player): 
chunkLoader(chunkLoader), player(player) {
    pos = player.getPos() + Vec3(0, 1.5, 0);
}

void Camera::update() {
    pos = player.getPos() + Vec3(0, 1.5, 0);

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
