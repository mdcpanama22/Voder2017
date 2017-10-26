#include "../includes/Window.h"

namespace gui {

    Window::Window(const char* ptitle, const int& pwidth, const int& pheight)
        : winWidth(pwidth), winHeight(pheight) 
    {
            
        window = glfwCreateWindow(winWidth, winHeight, ptitle, NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            std::cout << "ERROR: Could NOT create window!" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        glfwMakeContextCurrent(window);
        
    }
    
    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
    bool Window::windowShouldClose() {
        return glfwWindowShouldClose(window);
    }
    
    void Window::update() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    void Window::clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    }
        
    GLFWwindow* Window::getWindowPtr() {
        return window;
    }
    
    const int& Window::getMouseX() {
        return mouseX;    
    }
    
    const int& Window::getMouseY() {
        return mouseY;
    }
    
    const int& Window::getWindowWidth() {
        return winWidth;
    }
    
    const int& Window::getwindowHeight() {
        return winHeight;
    }

}