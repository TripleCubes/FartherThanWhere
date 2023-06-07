#include <Codes/Entities/player.h>

#include <Codes/settings.h>
#include <Codes/Chunk/chunkLoader.h>
#include <Codes/Time/time.h>
#include <vector>
#include <cmath>
#include <Codes/print.h>

Player::Player(const Settings &settings, const ChunkLoader &chunkLoader): 
settings(settings), chunkLoader(chunkLoader) {
    std::vector<float> playerVerticies = {
//      Pos                  Normal            UV
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

    internalVelocityCap = 15;
    internalVelocityAddAmount = 100;
    internalVelocitySlowDownAmount = 60;

    externalVelocityCap = 100;
    externalVelocitySlowDownAmount = 7;
    gravityVelocityAddAmount = 50;
    jumpVelocityAddAmount = 15;
}

void Player::setPos(Vec3 pos) {
    this->pos = pos;
}

Vec3 Player::getPos() const {
    return pos;
}

void Player::update() {
    if (!settings.isCollisionEnabled()) {
        // Keep some Y axis inertia when changed to no collision mode (since collision mode is also flying)
        addVelocity(internalVelocity, Vec3(0, 1, 0), externalVelocity.y, internalVelocityCap);
        externalVelocity = Vec3(0, 0, 0);

        pos += internalVelocity * Time::getDeltaTime();

        slowDownVelocity(internalVelocity, internalVelocitySlowDownAmount * Time::getDeltaTime());
        slowDownVelocity(externalVelocity, externalVelocitySlowDownAmount * Time::getDeltaTime());

        return;
    }

    if (settings.isFlying()) {
        // Keep some Y axis inertia when changed to flying mode
        addVelocity(internalVelocity, Vec3(0, 1, 0), externalVelocity.y, internalVelocityCap);
        externalVelocity = Vec3(0, 0, 0);

        moveAxis(internalVelocity.x * Time::getDeltaTime(), Axis::X);
        moveAxis(internalVelocity.y * Time::getDeltaTime(), Axis::Y);
        moveAxis(internalVelocity.z * Time::getDeltaTime(), Axis::Z);

        slowDownVelocity(internalVelocity, internalVelocitySlowDownAmount * Time::getDeltaTime());
        slowDownVelocity(externalVelocity, externalVelocitySlowDownAmount * Time::getDeltaTime());

        return;
    }

    addGravity();

    moveAxis((internalVelocity.x + externalVelocity.x) * Time::getDeltaTime(), Axis::X);
    moveAxis((internalVelocity.y + externalVelocity.y) * Time::getDeltaTime(), Axis::Y);
    moveAxis((internalVelocity.z + externalVelocity.z) * Time::getDeltaTime(), Axis::Z);

    slowDownVelocity(internalVelocity, internalVelocitySlowDownAmount * Time::getDeltaTime());
    slowDownVelocity(externalVelocity, externalVelocitySlowDownAmount * Time::getDeltaTime());
}

void Player::draw() const {
    mesh.draw();
}

void Player::move(Vec3 dir) {
    addVelocity(internalVelocity, dir, internalVelocityAddAmount * Time::getDeltaTime(), internalVelocityCap);
}

void Player::jump() {
    if (settings.isCollisionEnabled() && !settings.isFlying() && onGround) {
        addVelocity(externalVelocity, Vec3(0, 1, 0), jumpVelocityAddAmount, externalVelocityCap);
    }
}

void Player::moveAxis(float moveAmount, Axis axis) {
    if (moveAmount == 0) {
        return;
    }

    if (axis == Axis::Y) {
        onGround = false;
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
            internalVelocity.x = 0;
            externalVelocity.x = 0;
            break;
        
        case Axis::Y:
            pos.y = floor(nextPos.y + 0.5);
            internalVelocity.y = 0;
            externalVelocity.y = 0;
            break;

        case Axis::Z:
            pos.z = floor(nextPos.z) + 0.5;
            internalVelocity.z = 0;
            externalVelocity.z = 0;
            break;

        default:
            break;
        }
    };

    for (Vec3 collisionCheckPos: collisionCheckPosList) {
        if (chunkLoader.getBlock(IntPos(collisionCheckPos))) {
            collisionSnap();

            if (axis == Axis::Y && moveAmount < 0) {
                onGround = true;
            }

            return;
        }
    }

    pos = nextPos;
}

void Player::slowDownVelocity(Vec3 &velocity, float velocitySlowDownAmount) {
    if (velocitySlowDownAmount >= velocity.length()) {
        velocity = Vec3(0, 0, 0);
    } else {
        velocity -= velocity.normalize() * velocitySlowDownAmount;
    }
}

void Player::addVelocity(Vec3 &velocity, Vec3 velocityAddDir, float velocityAddAmount, float velocityCap) {
    velocity += velocityAddDir.normalize() * velocityAddAmount;

    if (velocity.length() > velocityCap) {
        velocity = velocity.normalize() * velocityCap;
    }
}

void Player::addGravity() {
    addVelocity(externalVelocity, Vec3(0, -1, 0), gravityVelocityAddAmount * Time::getDeltaTime(), externalVelocityCap);
}

Player::~Player() {
    mesh.release();
}
