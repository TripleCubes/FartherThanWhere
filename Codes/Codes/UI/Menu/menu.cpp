#include <Codes/UI/Menu/menu.h>

#include <Codes/UI/ui.h>
#include <Codes/UI/Menu/menuManager.h>
#include <Codes/Types/color.h>

#include <Codes/Debug/print.h>

extern int currentWindowWidth;
extern int currentWindowHeight;

//
const float PADDING_LEFT = 50;
const float PADDING_TOP = 50;
const float UIELEMENT_YCURSOR_STEP = 60;

Menu::Menu(const std::string &menuTitle): menuTitle(menuTitle), openMenuButton(OpenMenuButton(menuTitle)) {
    uiElementYCursor = 100;
}

void Menu::addSelfSharedPtr(std::shared_ptr<Menu> selfSharedPtr) {
    this->selfSharedPtr = selfSharedPtr;
    openMenuButton.addMenuSharedPtr(this->selfSharedPtr);

    selfSharedPtrAdded = true;
}

void Menu::addUpperLevelMenu(std::shared_ptr<Menu> upperLevelMenu) {
    this->upperLevelMenu = upperLevelMenu;
    root = false;
    getFullMenuTitle();
}

void Menu::addUIElement(const MenuUIElement &uiElement) {
    std::shared_ptr<MenuUIElement> uiElementPtr = std::make_shared<MenuUIElement>(uiElement);
    uiElementPtr->setUIElementRelativePos(PADDING_LEFT, uiElementYCursor);

    uiElementList.push_back(uiElementPtr);

    uiElementYCursor += UIELEMENT_YCURSOR_STEP;
}

void Menu::addSubMenu(const std::string &subMenuTitle) {
    if (!selfSharedPtrAdded) {
        PRINTLN("Menu::addSubMenu(): selfSharedPtr not added");
        return;
    }

    std::shared_ptr<Menu> subMenu = std::make_shared<Menu>(subMenuTitle);
    subMenu->addSelfSharedPtr(subMenu);
    subMenu->addUpperLevelMenu(this->selfSharedPtr);
    subMenu->setUIElementRelativePos(PADDING_LEFT, uiElementYCursor);
    subMenu->menuDepth = this->menuDepth + 1;

    subMenuList.push_back(subMenu);
    uiElementList.push_back(subMenu);

    uiElementYCursor += UIELEMENT_YCURSOR_STEP;
}

void Menu::getFullMenuTitle() {
    if (root) {
        PRINTLN("Menu::getFullMenuTitle(): cant call getFullMenuTitle() on root menu");
        return;
    }

    fullMenuTitle = menuTitle;
    Menu *currentMenu = upperLevelMenu.get();
    while (!currentMenu->root) {
        fullMenuTitle = currentMenu->menuTitle + "  >  " + fullMenuTitle;
        currentMenu = currentMenu->upperLevelMenu.get();
    }
}

Menu *Menu::getSubMenu(const std::string &subMenuTitle) const {
    for (const auto &subMenu: subMenuList) {
        if (subMenu->menuTitle == subMenuTitle) {
            return subMenu.get();
        }
    }

    PRINTLN("Menu::getSubMenu(): submenu not found");
    return NULL;
}

void Menu::open() const {
    MenuManager::open(selfSharedPtr);
}

void Menu::goBackOneMenu() const {
    if (root || menuDepth == 1) {
        MenuManager::closeAllMenus();
        return;
    }

    upperLevelMenu->open();
}

void Menu::updateMenu() {
    for (const auto &uiElement: uiElementList) {
        uiElement->updateUIElement();
    }
}

void Menu::drawMenu() const {
    UI::drawRectWH(0, 0, currentWindowWidth, currentWindowHeight, Color(0, 0, 0, 0.3));
    UI::drawTextBox(PADDING_LEFT, PADDING_TOP, fullMenuTitle, Color(1, 1, 1, 1));

    for (const auto &uiElement: uiElementList) {
        uiElement->drawUIElement();
    }
}

void Menu::updateUIElement() {
    openMenuButton.updateUIElement();
}

void Menu::drawUIElement() const {
    openMenuButton.drawUIElement();
}

void Menu::setUIElementRelativePos(float x, float y) {
    openMenuButton.setUIElementRelativePos(x, y);
}
