#pragma once

namespace core { namespace buffers {

	class IBO {
	private:

		unsigned int iboID;

	public:

		IBO();
		~IBO();

		/*BINDS the VAO*/
		void bind();

		/*UNBINDS the VAO*/
		void unbind();

		/*NOTE: this function will add the buffer data to the currently bound VBO, this will NOT bind one for you*/
		static void addBufferData(int size,
								  const void* data,
								  unsigned int usage);

	private:



	};

} }