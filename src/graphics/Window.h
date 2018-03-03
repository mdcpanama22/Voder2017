#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../math/vec2.h"
#include "../math/vec4.h"

#define MAX_KEYS    1024
#define MAX_BUTTONS 32

namespace core { namespace graphics {

	class Window {
	private:

		GLFWwindow* window;
		math::vec2 windowSize;
		math::vec2 mousePos = math::vec2(0, 0);
		const char* title;

		bool keys[MAX_KEYS];
		bool buttons[MAX_BUTTONS];

	public:

		Window(const int& pwidth, const int& pheight, const char* ptitle, bool fullScreen, bool vSync);
		~Window();

		bool windowShouldClose() const ;

		const math::vec2& getWindowSize() const ;
		const math::vec2& getMousePos() const ;

		void update() const ;
		void clear() const ;
		void setClearColor(const math::vec4& color) const ;
		bool isKeyPressed(unsigned int keycode) const ;
		bool isButtonPressed(unsigned int keycode) const ;


	private:
	
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void window_size_callback(GLFWwindow* window, int width, int height);




	};

} }