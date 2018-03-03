#pragma once
#include <iostream>
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec3.h"
#include "../math/mat4.h"

namespace core { namespace graphics {

	class Shader {
	private:

		unsigned int shaderID;

	public:

		Shader(const std::string& filepath);
		~Shader();

		void bind();
		void unbind();

		int getUniformLocation(const char* identifier);

		static void setUniformVec2(const int& location, const math::vec2& data);
		static void setUniformVec3(const int& location, const math::vec3& data);
		static void setUniformVec4(const int& location, const math::vec4& data);
		static void setUniformMat4(const int& location, const math::mat4& data);


	private:



	};


} }