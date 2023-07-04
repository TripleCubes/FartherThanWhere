#ifndef BUTTON_H
#define BUTTON_H

#include <Codes/Types/color.h>
#include <string>

class Button {
public:
    Button(const std::string &buttonTitle, bool isRelativePos, float x, float y, float w, float h, 
                Color buttonColor, Color textColor);

    void update();
    void draw() const;

    void setPos(float x, float y);

    bool pressed();
    bool justPressed();

private:
    std::string buttonTitle;
    bool isRelativePos = false;
    float x = 0;
    float y = 0;
    float w = 100;
    float h = 100;
    Color buttonColor;
    Color textColor;

    bool hovered = false;
    bool status_pressed = false;
    bool status_justPressed = false;
};

#endif
