#ifndef MENU_H
#define MENU_H

#include <Codes/UI/Menu/MenuUIElement.h>
#include <Codes/UI/Menu/openMenuButton.h>
#include <string>
#include <vector>
#include <memory>

class Menu: public MenuUIElement {
public:
    Menu(const std::string &menuTitle);
    void addSelfSharedPtr(std::shared_ptr<Menu> selfSharedPtr);

    void addUIElement(const MenuUIElement &uiElement);
    void addSubMenu(const std::string &subMenuTitle);
    Menu *getSubMenu(const std::string &subMenuTitle) const;
    void open() const;
    void goBackOneMenu() const;
    void updateMenu();
    void drawMenu() const;

    void updateUIElement() override;
    void drawUIElement() const override;
    void setUIElementRelativePos(float x, float y) override;

private:
    void addUpperLevelMenu(std::shared_ptr<Menu> upperLevelMenu);

    std::string menuTitle;
    std::vector<std::shared_ptr<MenuUIElement>> uiElementList;
    std::vector<std::shared_ptr<Menu>> subMenuList;
    bool root = true;
    int menuDepth = 0;
    std::shared_ptr<Menu> upperLevelMenu;
    bool selfSharedPtrAdded = false;
    std::shared_ptr<Menu> selfSharedPtr;

    void getFullMenuTitle();
    std::string fullMenuTitle;

    OpenMenuButton openMenuButton;

    float uiElementYCursor = 100;
};

#endif
