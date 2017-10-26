#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "includes/Window.h"
#include <iostream>

using namespace gui;

int main() {
    
    
    if (!glfwInit()) {
        std::cout << "GLFW could NOT init!" << std::endl;
        return -1;
    }
    
    Window* win = new Window("Title", 640, 480);
    
    glewInit();
    
    while (!win->windowShouldClose())
    {
       win->clear();
       win->update();
    }
    
    delete win;
    
    return 0;
    
}