#include "include/Window.h"

namespace gui {

	bool Window::keys[MAX_KEYS];
	bool Window::buttons[MAX_BUTTONS];
	double Window::mouseX, Window::mouseY;
    
    Window::Window(const char* ptitle, const int& pwidth, const int& pheight)
        : title(ptitle), winWidth(pwidth), winHeight(pheight) 
    {

		// Initialize GLFW 
		if (!glfwInit()) {
			std::cerr << "GLFW could NOT init!" << std::endl;
			exit(EXIT_FAILURE);
		}

		glfwSetErrorCallback(error_callback);

        window = glfwCreateWindow(winWidth, winHeight, title, NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            std::cout << "ERROR: Could NOT create window!" << std::endl;
            exit(EXIT_FAILURE);
        }
        
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, this);
		glfwSwapInterval(1);

		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, mouse_cursor_callback);
		glfwSetWindowSizeCallback(window, window_resize_callback);
        
        for (int i = 0; i < MAX_KEYS; ++i) {
            keys[i] = false;
        }
		for (int i = 0; i < MAX_BUTTONS; ++i) {
			buttons[i] = false;
		}

		glewInit();
		//if (glewInit() != GLEW_OK) {
		//	std::cout << "GLEW failed to initialize!" << std::endl;
		//	exit(EXIT_FAILURE);
		//}
        
    }
    
    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
    bool Window::windowShouldClose() {
        return glfwWindowShouldClose(window);
    }

	void Window::setWindowShouldClose(int val) {
		glfwSetWindowShouldClose(window, val);
	}
    
    void Window::update() {
        glfwSwapBuffers(window);
        glfwPollEvents();
	}
    
    void Window::clear() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    bool Window::isKeyPressed(unsigned int keycode) {
		if (keycode > MAX_KEYS)
			return false;
		return keys[keycode];
    }

	bool Window::isButtonPressed(unsigned int keycode) {
		if (keycode > MAX_BUTTONS)
			return false;
		return buttons[keycode];
	}

	void Window::getMousePos(double& x, double& y) {
		x = mouseX;
		y = mouseY;
	}

	void Window::error_callback(int error, const char * description) {
		std::cerr << "Error: " << description << std::endl;
	}

	void Window::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->keys[key] = (action != GLFW_RELEASE);
		std::cout << "Pressed!" << std::endl;
	}
	
	void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->buttons[button] = (action != GLFW_RELEASE);
	}
	
	void Window::mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos) {
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->mouseX = xpos;
		win->mouseY = ypos;
	}
	
	void Window::window_resize_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

}
