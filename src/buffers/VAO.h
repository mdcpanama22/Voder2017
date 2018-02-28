#pragma once

namespace core { namespace buffers {

	class VAO {
	private:

		unsigned int vaoID;


	public:

		VAO();
		~VAO();

		/*BINDS the VAO*/
		void bind();

		/*UNBINDS the VAO*/
		void unbind();

		/*NOTE: this function will add the vertex atrib to the currently bound VAO, this will NOT bind one for you*/
		static void addVertexAtribPointer(unsigned int index, 
										  int size, 
										  unsigned int type, 
										  bool normalized, 
										  int stride, 
										  const void* pointer);

	private:



	};

} }