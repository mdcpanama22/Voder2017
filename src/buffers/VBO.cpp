#include <GL/glew.h>
#include "VBO.h"
#include "../debuging.h"

namespace core { namespace buffers {

	VBO::VBO() {
		GLCall(glGenBuffers(1, &vboID));
	}

	VBO::~VBO() {
		GLCall(glDeleteBuffers(1, &vboID));
	}

	void VBO::bind() {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vboID));
	}

	void VBO::unbind() {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VBO::addBufferData(int size, 
							const void* data, 
							unsigned int usage)
	{
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
	}

} }