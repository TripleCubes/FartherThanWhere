#include <Codes/UI/Menu/menuManager.h>

#include <GLFW/glfw3.h>

extern GLFWwindow *glfwWindow;
extern int currentWindowWidth;
extern int currentWindowHeight;
extern bool mouseLocked;

bool MenuManager::allMenusClosed = true;
std::shared_ptr<Menu> MenuManager::currentMenu;

void MenuManager::open(std::shared_ptr<Menu> menu) {
    currentMenu = menu;
    allMenusClosed = false;
    
    if (mouseLocked) {
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPos(glfwWindow, (double)currentWindowWidth/2, (double)currentWindowHeight/2);
        mouseLocked = false;
    }
}

void MenuManager::closeAllMenus() {
    allMenusClosed = true;

    if (!mouseLocked) {
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseLocked = true;
    }
}

void MenuManager::goBackOneMenu() {
    if (!allMenusClosed) {
        currentMenu->goBackOneMenu();
    }
}

void MenuManager::update() {
    if (!allMenusClosed) {
        currentMenu->updateMenu();
    }
}

void MenuManager::draw() {
    if (!allMenusClosed) {
        currentMenu->drawMenu();
    }
}

bool MenuManager::isAllMenusClosed() {
    return allMenusClosed;
}
