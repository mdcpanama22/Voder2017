#include <iostream>
#include "GLFW/glfw3.h"
#include "includes/Window.h"

using namespace gui;

int main() {
    
    
    if (!glfwInit()) {
        std::cout << "GLFW could NOT init!" << std::endl;
        return -1;
    }
    
    Window* win = new Window("Title", 640, 480);
    
    while (!win->windowShouldClose())
    {
       
    }
    
    delete win;
    
    return 0;
    
}