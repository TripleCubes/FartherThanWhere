#include <Codes/Entities/player.h>

#include <Codes/settings.h>
#include <Codes/Chunk/chunkLoader.h>
#include <vector>
#include <cmath>
#include <Codes/print.h>

Player::Player(const Settings &settings, const ChunkLoader &chunkLoader): 
settings(settings), chunkLoader(chunkLoader) {
    std::vector<float> playerVerticies = {
        -0.5,    2, -0.5,    0,    1,    0,    0,    1, // A
         0.5,    2,  0.5,    0,    1,    0,    1,    0, // C
        -0.5,    2,  0.5,    0,    1,    0,    0,    0, // D

        -0.5,    2, -0.5,    0,    1,    0,    0,    1, // A
         0.5,    2, -0.5,    0,    1,    0,    1,    1, // B
         0.5,    2,  0.5,    0,    1,    0,    1,    0, // C

        -0.5,    0, -0.5,    0,   -1,    0,    1,    1, // E
        -0.5,    0,  0.5,    0,   -1,    0,    1,    0, // H
         0.5,    0,  0.5,    0,   -1,    0,    0,    0, // G

        -0.5,    0, -0.5,    0,   -1,    0,    1,    1, // E
         0.5,    0,  0.5,    0,   -1,    0,    0,    0, // G
         0.5,    0, -0.5,    0,   -1,    0,    0,    1, // F

        -0.5,    2, -0.5,   -1,    0,    0,    0,    1, // A
        -0.5,    2,  0.5,   -1,    0,    0,    1,    1, // D
        -0.5,    0,  0.5,   -1,    0,    0,    1,    0, // H

        -0.5,    2, -0.5,   -1,    0,    0,    0,    1, // A
        -0.5,    0,  0.5,   -1,    0,    0,    1,    0, // H
        -0.5,    0, -0.5,   -1,    0,    0,    0,    0, // E

         0.5,    2, -0.5,    1,    0,    0,    1,    1, // B
         0.5,    0,  0.5,    1,    0,    0,    0,    0, // G
         0.5,    2,  0.5,    1,    0,    0,    0,    1, // C

         0.5,    2, -0.5,    1,    0,    0,    1,    1, // B
         0.5,    0, -0.5,    1,    0,    0,    1,    0, // F
         0.5,    0,  0.5,    1,    0,    0,    0,    0, // G

        -0.5,    2,  0.5,    0,    0,    1,    0,    1, // D
         0.5,    2,  0.5,    0,    0,    1,    1,    1, // C
        -0.5,    0,  0.5,    0,    0,    1,    0,    0, // H

         0.5,    2,  0.5,    0,    0,    1,    1,    1, // C
         0.5,    0,  0.5,    0,    0,    1,    1,    0, // G
        -0.5,    0,  0.5,    0,    0,    1,    0,    0, // H

        -0.5,    2, -0.5,    0,    0,   -1,    1,    1, // A
        -0.5,    0, -0.5,    0,    0,   -1,    1,    0, // E
         0.5,    2, -0.5,    0,    0,   -1,    0,    1, // B

         0.5,    2, -0.5,    0,    0,   -1,    0,    1, // B
        -0.5,    0, -0.5,    0,    0,   -1,    1,    0, // E
         0.5,    0, -0.5,    0,    0,   -1,    0,    0, // F
    };
    mesh.set3d(playerVerticies);

    pos = Vec3(0, 30, 0);
}

void Player::setPos(Vec3 pos) {
    this->pos = pos;
}

Vec3 Player::getPos() const {
    return pos;
}

void Player::draw() const {
    mesh.draw();
}

void Player::move(Vec3 dir) {
    if (settings.isCollisionDisabled()) {
        pos += dir * moveSpeed;
        return;
    }

    dir = dir.normalize();

    moveAxis(dir.y * moveSpeed, Axis::Y);
    moveAxis(dir.x * moveSpeed, Axis::X);
    moveAxis(dir.z * moveSpeed, Axis::Z);
}

void Player::moveAxis(float moveAmount, Axis axis) {
    if (moveAmount == 0) {
        return;
    }

    auto getNextPos = [axis, moveAmount, this]() -> Vec3 {
        switch (axis) {
        case Axis::X:
            return pos + Vec3(moveAmount, 0, 0);
            break;

        case Axis::Y:
            return pos + Vec3(0, moveAmount, 0);
            break;

        case Axis::Z:
            return pos + Vec3(0, 0, moveAmount);
            break;

        default:
            return Vec3(0, 0, 0);
            break;
        }
    };

    Vec3 nextPos = getNextPos();

    std::vector<Vec3> collisionCheckPosList;

    switch (axis) {
    case Axis::X:
        if (moveAmount > 0) {
            collisionCheckPosList = {
                nextPos + Vec3(0.4999, 0.0001,  0.4999),
                nextPos + Vec3(0.4999, 0.0001, -0.4999),
                nextPos + Vec3(0.4999, 1     ,  0.4999),
                nextPos + Vec3(0.4999, 1     , -0.4999),
                nextPos + Vec3(0.4999, 1.9999,  0.4999),
                nextPos + Vec3(0.4999, 1.9999, -0.4999),
            };
        } else {
            collisionCheckPosList = {
                nextPos + Vec3(-0.4999, 0.0001,  0.4999),
                nextPos + Vec3(-0.4999, 0.0001, -0.4999),
                nextPos + Vec3(-0.4999, 1     ,  0.4999),
                nextPos + Vec3(-0.4999, 1     , -0.4999),
                nextPos + Vec3(-0.4999, 1.9999,  0.4999),
                nextPos + Vec3(-0.4999, 1.9999, -0.4999),
            };
        }
        break;

    case Axis::Y:
        if (moveAmount > 0) {
            collisionCheckPosList = {
                nextPos + Vec3( 0.4999, 1.9999, -0.4999),
                nextPos + Vec3( 0.4999, 1.9999,  0.4999),
                nextPos + Vec3(-0.4999, 1.9999, -0.4999),
                nextPos + Vec3(-0.4999, 1.9999,  0.4999),
            };
        } else {
            collisionCheckPosList = {
                nextPos + Vec3( 0.4999, 0.0001, -0.4999),
                nextPos + Vec3( 0.4999, 0.0001,  0.4999),
                nextPos + Vec3(-0.4999, 0.0001, -0.4999),
                nextPos + Vec3(-0.4999, 0.0001,  0.4999),
            };
        }
        break;

    case Axis::Z:
        if (moveAmount > 0) {
            collisionCheckPosList = {
                nextPos + Vec3( 0.4999, 0.0001, 0.4999),
                nextPos + Vec3(-0.4999, 0.0001, 0.4999),
                nextPos + Vec3( 0.4999, 1     , 0.4999),
                nextPos + Vec3(-0.4999, 1     , 0.4999),
                nextPos + Vec3( 0.4999, 1.9999, 0.4999),
                nextPos + Vec3(-0.4999, 1.9999, 0.4999),
            };
        } else {
            collisionCheckPosList = {
                nextPos + Vec3( 0.4999, 0.0001, -0.4999),
                nextPos + Vec3(-0.4999, 0.0001, -0.4999),
                nextPos + Vec3( 0.4999, 1     , -0.4999),
                nextPos + Vec3(-0.4999, 1     , -0.4999),
                nextPos + Vec3( 0.4999, 1.9999, -0.4999),
                nextPos + Vec3(-0.4999, 1.9999, -0.4999),
            };
        }
        break;

    default: 
        break;
    }

    auto collisionSnap = [axis, nextPos, this]() {
        switch (axis) {
        case Axis::X:
            pos.x = floor(nextPos.x) + 0.5;
            break;
        
        case Axis::Y:
            pos.y = floor(nextPos.y + 0.5);
            break;

        case Axis::Z:
            pos.z = floor(nextPos.z) + 0.5;
            break;

        default:
            break;
        }
    };

    for (Vec3 collisionCheckPos: collisionCheckPosList) {
        if (chunkLoader.getBlock(IntPos(collisionCheckPos))) {
            collisionSnap();
            return;
        }
    }

    pos = nextPos;
}

Player::~Player() {
    mesh.release();
}
