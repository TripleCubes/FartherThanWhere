#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Codes/world.h>
#include <Codes/game.h>
#include <Codes/input.h>
#include <Codes/Time/time.h>
#include <Codes/UI/ui.h>

#include <Codes/GraphicEffects/boxBlur.h>

#include <Codes/print.h>

extern GLFWwindow *glfwWindow;
extern int currentWindowWidth;
extern int currentWindowHeight;
extern bool mouseLock;

float lastMousePosX = 0;
float lastMousePosY = 0;
bool resetMouseMoveOffset = true;

void onMouseMove(double mousex, double mousey) {
    if (!mouseLock) {
        resetMouseMoveOffset = true;
        return;
    }

    if (resetMouseMoveOffset) {
        lastMousePosX = mousex;
        lastMousePosY = mousey;
        Input::updateMouseMoveOffset(0, 0);
        resetMouseMoveOffset = false;
        return;
    }
    
    Input::updateMouseMoveOffset(mousex - lastMousePosX, mousey - lastMousePosY);
    lastMousePosX = mousex;
    lastMousePosY = mousey;
}

void onMouseMove(GLFWwindow* window, double mousex, double mousey) {
    onMouseMove(mousex, mousey);
}

void initOpenGl() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindow = glfwCreateWindow(800, 500, "Farther than where", NULL, NULL);
    if (glfwWindow == NULL) {
        PRINTLN("failed to create window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(glfwWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        PRINTLN("failed to initialize GLAD");
        return;
    }

    glViewport(0, 0, 800, 500);

    glfwSetCursorPosCallback(glfwWindow, onMouseMove);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main() {
    initOpenGl();

    {
        Input::init();
        UI::init();

        GraphicEffects::BoxBlur::init();

        World world;
        Game game(world);

        while (!glfwWindowShouldClose(glfwWindow)) {
            Time::setFrameStartTime();

            glfwPollEvents();
            Input::update();

            game.update();
            world.update();
            
            game.draw();
            UI::draw();
            glfwSwapBuffers(glfwWindow);

            Time::syncFrame();
            Time::update();
        }

        GraphicEffects::BoxBlur::release();

        UI::release();
    }

    PRINTLN("Game closed successfully");

    return 0;
}
