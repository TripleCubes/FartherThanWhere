#include <Codes/input.h>

#include <GLFW/glfw3.h>
#include <Codes/Debug/print.h>

extern GLFWwindow *glfwWindow;

std::unordered_map<std::string, Input::Key> Input::keys;
bool Input::leftPressed = false;
bool Input::leftJustPressed = false;
bool Input::rightPressed = false;
bool Input::rightJustPressed = false;
float Input::mouseMoveOffsetX = 0;
float Input::mouseMoveOffsetY = 0;
bool Input::resetMouseMoveOffset = false;

void Input::addKey(const std::string &keyString, int glfwKey) {
    Key key = {glfwKey, false, false};
    keys.insert(std::make_pair(keyString, key));
}

void Input::updateMouseMoveOffset(float x, float y) {
    mouseMoveOffsetX = x;
    mouseMoveOffsetY = y;
}

void Input::init() {
    addKey("ESC", GLFW_KEY_ESCAPE);
    addKey("T", GLFW_KEY_T);
    addKey("P", GLFW_KEY_P);
    addKey("M", GLFW_KEY_M);
    addKey("N", GLFW_KEY_N);
    addKey("F", GLFW_KEY_F);
    addKey("B", GLFW_KEY_B);
    addKey("V", GLFW_KEY_V);
    
    addKey("W", GLFW_KEY_W);
    addKey("A", GLFW_KEY_A);
    addKey("S", GLFW_KEY_S);
    addKey("D", GLFW_KEY_D);

    addKey("SPACE", GLFW_KEY_SPACE);
    addKey("LEFT_SHIFT", GLFW_KEY_LEFT_SHIFT);

    addKey("ESC", GLFW_KEY_ESCAPE);
}

void Input::update() {
    for (auto &i: keys) {
        if (glfwGetKey(glfwWindow, i.second.glfwKey) == GLFW_PRESS) {
            if (!i.second.pressed) {
                i.second.justPressed = true;
            } else {
                i.second.justPressed = false;
            }
            i.second.pressed = true;
        } else {
            i.second.pressed = false;
            i.second.justPressed = false;
        }
    }

    if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!leftPressed) {
            leftJustPressed = true;
        } else {
            leftJustPressed = false;
        }
        leftPressed = true;
    } else {
        leftPressed = false;
        leftJustPressed = false;
    }

    if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (!rightPressed) {
            rightJustPressed = true;
        } else {
            rightJustPressed = false;
        }
        rightPressed = true;
    } else {
        rightPressed = false;
        rightJustPressed = false;
    }

    if (resetMouseMoveOffset) {
        resetMouseMoveOffset = false;
        mouseMoveOffsetX = 0;
        mouseMoveOffsetY = 0;
    }
    if (mouseMoveOffsetX != 0 || mouseMoveOffsetY != 0) {
        resetMouseMoveOffset = true;
    }
}

bool Input::pressed(const std::string &key) {
    if (keys.find(key) == keys.end()) {
        PRINT("key");
        PRINT(key);
        PRINTLN("isnt added");
        return false;
    }
    return keys.at(key).pressed;
}

bool Input::justPressed(const std::string &key) {
    if (keys.find(key) == keys.end()) {
        PRINT("key");
        PRINT(key);
        PRINTLN("isnt added");
        return false;
    }
    return keys.at(key).justPressed;
}

Vec2 Input::getMousePos() {
    double x = 0;
    double y = 0;
    glfwGetCursorPos(glfwWindow, &x, &y);
    return Vec2((float)x, (float)y);
}

Vec2 Input::getMouseMoveOffset() {
    return Vec2(mouseMoveOffsetX, mouseMoveOffsetY);
}

bool Input::pressed(MouseButton mouseButton) {
    if (mouseButton == MouseButton::LEFT) {
        return leftPressed;
    }
    return rightPressed;
}

bool Input::justPressed(MouseButton mouseButton) {
    if (mouseButton == MouseButton::LEFT) {
        return leftJustPressed;
    }
    return rightJustPressed;
}
