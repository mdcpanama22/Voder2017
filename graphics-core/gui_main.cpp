#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "includes/Window.h"
#include <iostream>

using namespace gui;

//callbacks
void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
    
    
    if (!glfwInit()) {
        std::cout << "GLFW could NOT init!" << std::endl;
        return -1;
    }
    
    Window* win = new Window("Title", 640, 480);
    
    glewInit();
    
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(win->getWindowPtr(), key_callback);
    
    while (!win->windowShouldClose())
    {
       win->clear();
       win->update();
    }
    
    delete win;
    
    return 0;
    
}

void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    Window* w = (Window*)glfwGetWindowUserPointer(window);
    
    if (action != GLFW_RELEASE)
        w->setKey(key, true);
    else 
        w->setKey(key, false);
    
}