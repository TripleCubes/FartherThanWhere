#include <Codes/UI/button.h>

#include <Codes/UI/ui.h>
#include <Codes/input.h>
#include <Codes/Types/vec2.h>

Button::Button(const std::string &buttonTitle, bool isRelativePos, float x, float y, float w, float h, 
                Color buttonColor, Color textColor):
buttonTitle(buttonTitle), isRelativePos(isRelativePos), x(x), y(y), w(w), h(h), 
buttonColor(buttonColor), textColor(textColor) {}

void Button::update() {
    Vec2 mousePos = Input::getMousePos();
    
    hovered = false;
    if (mousePos.x >= x && mousePos.x <= x + w && mousePos.y >= y && mousePos.y <= y + h) {
        hovered = true;
    }

    status_pressed = false;
    status_justPressed = false;
    if (hovered) {
        if (Input::pressed(MouseButton::LEFT)) {
            status_pressed = true;
        }

        if (Input::justPressed(MouseButton::LEFT)) {
            status_justPressed = true;
        }
    }
}

void Button::draw() const {
    UI::drawRectWH(x, y, w, h, hovered? buttonColor + Color(0.1, 0.1, 0.1, 1) : buttonColor);
    UI::drawTextBox(x + 10, y + 5, buttonTitle, textColor);
}

void Button::setPos(float x, float y) {
    this->x = x;
    this->y = y;
}

bool Button::pressed() {
    return status_pressed;
}

bool Button::justPressed() {
    return status_justPressed;
}
