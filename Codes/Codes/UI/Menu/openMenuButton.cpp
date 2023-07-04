#include <Codes/UI/Menu/openMenuButton.h>

#include <Codes/UI/Menu/menu.h>
#include <Codes/Types/color.h>

#include <Codes/Debug/print.h>

const int BUTTON_WIDTH = 150;
const int BUTTON_HEIGHT = 40;
const Color BUTTON_COLOR = Color(1, 0.65, 0.57, 1);
const Color BUTTON_TEXT_COLOR = Color(1, 1, 1, 1);

OpenMenuButton::OpenMenuButton(const std::string &buttonLabel): 
button(buttonLabel, true, 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOR, BUTTON_TEXT_COLOR) {}

void OpenMenuButton::addMenuSharedPtr(std::shared_ptr<Menu> menuSharedPtr) {
    this->menuSharedPtr = menuSharedPtr;
    menuSharedPtrAdded = true;
}

void OpenMenuButton::updateUIElement() {
    if (!menuSharedPtrAdded) {
        PRINTLN("OpenMenuButton::updateUIElement(): menuSharedPtr not added");
        return;
    }

    button.update();

    if (button.justPressed()) {
        menuSharedPtr->open();
    }
}

void OpenMenuButton::drawUIElement() const {
    button.draw();
}

void OpenMenuButton::setUIElementRelativePos(float x, float y) {
    button.setPos(x, y);
}
