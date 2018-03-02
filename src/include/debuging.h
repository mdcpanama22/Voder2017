#pragma once
#include <iostream>
//NOTE: This should only be included in .cpp's please do not include this in a .h file

static bool GLLogError(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] ( " << error << " ) \n" << file << "\n" << line << ": " << function << std::endl << std::endl;
		return false;
	}
	return true;
}

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}


#define ASSERT(x) if(!(x)); //__debugbreak() is MSVC compiler intrinsic. and will NOT work in linux
#define GLCall(x) GLClearError();\
				  x;\
                  ASSERT(GLLogError(#x, __FILE__, __LINE__))
