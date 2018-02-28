#include <GL/glew.h>
#include "VAO.h"
#include "../debuging.h"

namespace core { namespace buffers {

	VAO::VAO() {
		GLCall(glGenVertexArrays(1, &vaoID));
	}

	VAO::~VAO() {

		/*
		BUG_REPORT: in main.cpp the vao is being deleted after the window closes.
		causing the subsequent calls to openGL functions to error and cause an infinite loop
		*/

		GLCall(glDeleteVertexArrays(1, &vaoID));

	}

	void VAO::bind() {
		GLCall(glBindVertexArray(vaoID));
	}

	void VAO::unbind() {
		GLCall(glBindVertexArray(0));
	}

	void VAO::addVertexAtribPointer(unsigned int index,
									int size,
									unsigned int type,
									bool normalized,
									int stride,
									const void* pointer)
	{
		GLCall(glEnableVertexAttribArray(index));
		GLCall(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
	}

} }