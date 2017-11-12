#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cstdlib>

namespace gui {
    
    #define MAX_KEYS 1024
	#define MAX_BUTTONS 32
    
    class Window {
    private:
    
        GLFWwindow* window;
		const char* title;
        int winWidth, winHeight;
        
        static bool keys[MAX_KEYS];
		static bool buttons[MAX_BUTTONS];
		static double mouseX, mouseY;
    
    
    public:

        Window(const char* ptitle, const int& pwidth, const int& pheight);
        ~Window();
        
        bool windowShouldClose();
		void setWindowShouldClose(int val);
        void update();
        void clear() const ;
        
		inline int getWidth() const { return winWidth; }
		inline int getHeight() const { return winHeight; }

		static void error_callback(int error, const char * description);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
		static void window_resize_callback(GLFWwindow* window, int width, int height);

		static bool isKeyPressed(unsigned int keycode);
		static bool isButtonPressed(unsigned int keycode);
		static void getMousePos(double& x, double& y);
        
    
    private:
    
    
    
  
    };    
    
}
