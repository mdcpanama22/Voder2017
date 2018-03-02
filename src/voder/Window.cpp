#include "include/Window.h"

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

        glfwSetWindowUserPointer(window, this);

        for (int i = 0; i < MAX_KEYS; ++i) {
            keys[i] = false;
        }


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

    void Window::setKey(int keycode, bool value) {
        keys[keycode] = value;
    }

    bool Window::isKeyPressed(int keycode) {
        return keys[keycode];
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
