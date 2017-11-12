#include <iostream>
#include "GLFW/glfw3.h"
#include <cstdlib>

namespace gui {
    
    #define MAX_KEYS 1024
    
    class Window {
    private:

    
        GLFWwindow* window;
        int winWidth, winHeight;
        int mouseX, mouseY;
        
        bool keys[MAX_KEYS];
    
    
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
        void setKey(int keycode, bool value);
        bool isKeyPressed(int keycode);
        
    
    
    private:
    
    
    
  
    };    
    
}
