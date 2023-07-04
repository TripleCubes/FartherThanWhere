#ifndef OPENMENUBUTTON_H
#define OPENMENUBUTTON_H

#include <Codes/UI/Menu/menuUIElement.h>
#include <Codes/UI/button.h>
#include <string>
#include <memory>

class Menu;

class OpenMenuButton: public MenuUIElement {
public:
    OpenMenuButton(const std::string &buttonLabel);

    void addMenuSharedPtr(std::shared_ptr<Menu> menuSharedPtr);

    void updateUIElement() override;
    void drawUIElement() const override;
    void setUIElementRelativePos(float x, float y) override;

private:
    Button button;

    bool menuSharedPtrAdded = false;
    std::shared_ptr<Menu> menuSharedPtr;
};

#endif
