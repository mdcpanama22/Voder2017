#include "Logging.h"

namespace core {

	HANDLE Log::console;
	int Log::colorAttrib;

	void Log::init() {
		console = GetStdHandle(STD_OUTPUT_HANDLE);
		colorAttrib = NOMINAL_COLOR;
	}

	void Log::testColors(int num) {

		// you can loop k higher to see more color choices
		for (colorAttrib = 1; colorAttrib < num; colorAttrib++)
		{
			// pick the colorattribute k you want
			SetConsoleTextAttribute(console, colorAttrib);
			std::cout << colorAttrib << " I want to be nice today!" << std::endl;
		}
	}

	void Log::printError(const char* text) {
		SetConsoleTextAttribute(console, ERROR_COLOR);
		std::cout << "[FeatherFall](  ERROR  ): " << text << std::endl;
		resetColor();
	}

	void Log::printWarning(const char* text) {
		SetConsoleTextAttribute(console, WARNING_COLOR);
		std::cout << "[FeatherFall]( WARNING ): " << text << std::endl;
		resetColor();
	}

	void Log::printText(const char* text) {
		SetConsoleTextAttribute(console, NOMINAL_COLOR);
		std::cout << "[FeatherFall]( NOMINAL ): " << text << std::endl;
		resetColor();
	}

	void Log::pause() {
		std::cin.get();
	}

	void Log::resetColor() {
		SetConsoleTextAttribute(console, NOMINAL_COLOR);
	}

}