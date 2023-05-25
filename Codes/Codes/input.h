#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <unordered_map>
#include <Codes/Types/vec2.h>

enum class MouseButton {
    LEFT,
    RIGHT,
};

class Input {
    friend void onMouseMove(double mousex, double mousey);

public:
    static void init();
    static void update();

    static bool pressed(const std::string &key);
    static bool justPressed(const std::string &key);

    static Vec2 getMousePos();
    static Vec2 getMouseMoveOffset();
    static bool pressed(MouseButton mouseButton);
    static bool justPressed(MouseButton mouseButton);

private:
    struct Key 
    {
        int glfwKey = 0;
        bool pressed = false;
        bool justPressed = false;
    };

    static std::unordered_map<std::string, Key> keys;
    static bool leftPressed;
    static bool leftJustPressed;
    static bool rightPressed;
    static bool rightJustPressed;

    static void addKey(const std::string &key, int glfwKey);

    static float mouseMoveOffsetX;
    static float mouseMoveOffsetY;
    static bool resetMouseMoveOffset;
    static void updateMouseMoveOffset(float x, float y);
};

#endif
