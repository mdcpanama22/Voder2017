#include <iostream>
#include "GLFW/glfw3.h"

namespace gui {
    
    class Window {
    private:

    
        GLFWwindow* window;
        int winWidth, winHeight;
        int mouseX, mouseY;
    
    
    public:
    
    
        Window(const char* ptitle, const int& pwidth, const int& pheight);
        ~Window();
        
        bool windowShouldClose();
        void update();
        void clear();
        
        
        GLFWwindow* getWindowPtr();
        const int& getMouseX();
        const int& getMouseY();
        const int& getWindowWidth();
        const int& getwindowHeight();
    
    
    private:
    
    
    
  
    };    
    
}