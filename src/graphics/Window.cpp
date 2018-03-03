#include "Window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
//#include "../logging/Logging.h"
#include "../include/debugging.h"

namespace core { namespace graphics {

	Window::Window(const int& pwidth, const int& pheight, const char* ptitle, bool fullScreen, bool vSync) 
		: windowSize(pwidth, pheight), mousePos(0, 0), title(ptitle)
	{
		//Init GLFW
		if (!glfwInit()) { //LOG THIS
			//Log::printError("GLFW failed to Initalize!");
			std::cout << "GLFW failed to init!" << std::endl;
			exit(EXIT_FAILURE);
		}

		//Set OpenGL type
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//Create Window
		window = glfwCreateWindow(windowSize.x, windowSize.y, title, (fullScreen) ? glfwGetPrimaryMonitor() : NULL , NULL);
		
		//Make sure window exists
		if (!window) {//LOG THIS
			//Log::printError("Window was not created!");
			std::cout << "Window was not created!" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		//Set Window Icon
		GLFWimage image;
		int w, h, bpp;
		unsigned char* rgb = stbi_load("res/icon/featherfall_icon.png", &w, &h, &bpp, 4);
		image.width = w;
		image.height = h;
		image.pixels = rgb;
		//glfwSetWindowIcon(window, 1, &image);
		//stbi_image_free(rgb);
		
		//Make OpenGL context Current
		glfwMakeContextCurrent(window);

		//Init GLEW
		if (glewInit() != GLEW_OK) {//LOG THIS
			//Log::printError("GLEW Failed to Initialize!");
			std::cout << "GLEW failed to Init!" << std::endl;
		}

		//Print OpenGL version (should remove this and make the Logger do this)
		//Log::printText((const char*)glGetString(GL_VERSION));

		//set the windowUserPointer so that this object can be referenced in the callbacks
		glfwSetWindowUserPointer(window, (void*)this); 

		//turns vSyn on if told to do so
		if (vSync) {
			glfwSwapInterval(1);
		}

		//Set Callbacks
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetWindowSizeCallback(window, window_size_callback);

		//Set all keys to default
		for (int i = 0; i < MAX_KEYS; ++i) {
			keys[i] = false;
		}

		//Set all buttons to default
		for (int i = 0; i < MAX_BUTTONS; ++i) {
			buttons[i] = false;
		}

	}

	Window::~Window() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	bool Window::windowShouldClose() const  {
		return glfwWindowShouldClose(window);
	}

	const math::vec2& Window::getWindowSize() const {
		return windowSize;
	}

	const math::vec2& Window::getMousePos() const {
		return mousePos;
	}

	void Window::update() const {
		glfwSwapBuffers(window);
		glfwPollEvents();
		if (isKeyPressed(GLFW_KEY_ESCAPE)) //quick escape for window closing
			glfwSetWindowShouldClose(window, true);
	}

	void Window::clear() const {
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void Window::setClearColor(const math::vec4& color) const {
		GLCall(glClearColor(color.x, color.y, color.z, color.w));
	}

	bool Window::isKeyPressed(unsigned int keycode) const {
		if (keycode >= MAX_KEYS)
			return false;
		return keys[keycode];
	}

	bool Window::isButtonPressed(unsigned int button) const {
		if (button >= MAX_BUTTONS)
			return false;
		return buttons[button];
	}

	void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->keys[key] = (action != GLFW_RELEASE);
	}

	void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->buttons[button] = (action != GLFW_RELEASE);
	}

	void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->mousePos.x = (float)xpos;
		win->mousePos.y = (float)ypos;
	}

	void Window::window_size_callback(GLFWwindow* window, int width, int height) {
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->windowSize.x = width;
		win->windowSize.y = height;
		GLCall(glViewport(0, 0, width, height));
	}


} }
