#include <GL/glew.h>
#include "IBO.h"
#include "../debuging.h"

namespace core { namespace buffers {

	IBO::IBO() {
		GLCall(glGenBuffers(1, &iboID));
	}

	IBO::~IBO() {
		GLCall(glDeleteBuffers(1, &iboID));
	}

	void IBO::bind() {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID));
	}

	void IBO::unbind() {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void IBO::addBufferData(int size,
							const void* data,
							unsigned int usage)
	{
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage));
	}

} }