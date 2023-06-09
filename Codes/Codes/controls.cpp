#include <Codes/controls.h>

#include <Codes/input.h>
#include <Codes/camera.h>
#include <Codes/settings.h>
#include <Codes/Types/vec2.h>
#include <Codes/Types/vec3.h>
#include <Codes/Raycast/blockRaycast.h>
#include <Codes/Chunk/chunkLoader.h>
#include <Codes/Entities/player.h>
#include <Codes/print.h>
#include <GLFW/glfw3.h>

extern GLFWwindow *glfwWindow;
extern int currentWindowWidth;
extern int currentWindowHeight;
extern bool mouseLock;

Controls::Controls(Settings &settings, Camera &camera, ChunkLoader &chunkLoader, Player &player): 
settings(settings), camera(camera), chunkLoader(chunkLoader), player(player) {}

void Controls::update() {
    updateSettings();
    updateCameraDir();
    updateMovements();
    placeBreakBlock();
}

void Controls::updateSettings() {
    if (Input::justPressed("T")) {
        settings.thirdPersonView = !settings.thirdPersonView;
    }

    if (Input::justPressed("M")) {
        settings.wireframeMode = !settings.wireframeMode;
    }

    if (Input::justPressed("N")) {
        settings.collisionEnabled = !settings.collisionEnabled;
        settings.flying = true;
    }

    if (Input::justPressed("F")) {
        if (!settings.collisionEnabled) {
            settings.collisionEnabled = true;
            settings.flying = false;
        } else {
            settings.flying = !settings.flying;
        }
    }

    if (Input::justPressed("ESC")) {
        mouseLock = !mouseLock;
        if (mouseLock) {
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPos(glfwWindow, (double)currentWindowWidth/2, (double)currentWindowHeight/2);
        }
    }
}

void Controls::updateCameraDir() {
    if (!mouseLock) {
        return;
    }

    Vec2 mouseMoveOffset = Input::getMouseMoveOffset();

    cameraRotationX -= mouseMoveOffset.y * 0.17;
    if (cameraRotationX > 89.99)
    {
        cameraRotationX = 89.99;
    }
    else if (cameraRotationX < -89.99)
    {
        cameraRotationX = -89.99;
    }
    cameraRotationY -= mouseMoveOffset.x * 0.17;

    camera.setDir(Vec3(0, 0, -1).rotateXY(cameraRotationX, cameraRotationY));
}

void Controls::updateMovements() {
    Vec3 moveDir(0, 0, 0);
    Vec3 cameraDir = camera.getDir();
    cameraDir.y = 0;
    cameraDir = cameraDir.normalize();
    Vec3 cameraDirRotated = cameraDir.rotateY(90);
    if (Input::pressed("W")) {
        moveDir += cameraDir;
    }
    if (Input::pressed("A")) {
        moveDir += cameraDirRotated;
    }
    if (Input::pressed("S")) {
        moveDir -= cameraDir;
    }
    if (Input::pressed("D")) {
        moveDir -= cameraDirRotated;
    }
    if (settings.isFlying() || !settings.isCollisionEnabled()) {
        if (Input::pressed("SPACE")) {
            moveDir.y += 1;
        }
        if (Input::pressed("LEFT_SHIFT")) {
            moveDir.y -= 1;
        }
    } else {
        if (Input::pressed("SPACE")) {
            player.jump();
        }
    }
    
    if (moveDir != Vec3(0, 0, 0)) {
        player.move(moveDir);
    }
}

void Controls::placeBreakBlock() {
    if (Input::justPressed(MouseButton::LEFT)) {
        const BlockRaycast::Result &blockRaycastResult = camera.getBlockRaycastResult();
        if (blockRaycastResult.found) {
            chunkLoader.breakBlock(blockRaycastResult.selectedPos);
        }
    }

    if (Input::justPressed(MouseButton::RIGHT)) {
        const BlockRaycast::Result &blockRaycastResult = camera.getBlockRaycastResult();
        if (blockRaycastResult.hasPlacingPos) {
            chunkLoader.placeBlock(blockRaycastResult.placingPos);
        }
    }
}
