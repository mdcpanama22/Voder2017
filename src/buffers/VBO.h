#pragma once

namespace core { namespace buffers {

	class VBO {
	private:
	
		unsigned int vboID;

	public:

		VBO();
		~VBO();

		/*BINDS the VBO*/
		void bind();

		/*UNBINDS the VBO*/
		void unbind();

		/*NOTE: this function will add the buffer data to the currently bound VBO, this will NOT bind one for you*/
		static void addBufferData(int size, 
								  const void* data, 
								  unsigned int usage);

	private:



	};

} }