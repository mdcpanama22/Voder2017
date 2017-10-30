#include "Window.h"

int main(int argc, char ** argv) {

	

	// Create Window
	gui::Window * win = new gui::Window("Windows-TestMain", 640, 480);

	// Core loop
	while (!win->windowShouldClose()) {
		win->clear();
		win->update();

		if (win->isKeyPressed(GLFW_KEY_ESCAPE))
			win->setWindowShouldClose(GLFW_TRUE);

	}

	delete win;
	return 0;
}

