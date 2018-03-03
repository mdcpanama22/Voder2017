#include <GL/glew.h>
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <string>
#include "../include/debugging.h"

namespace core { namespace graphics {

	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
		std::string GeometrySource;
	};
	static ShaderProgramSource parseShader(const std::string& filepath) {
		std::ifstream stream(filepath);

		enum class ShaderType {
			NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
		};

		std::string line;
		std::stringstream ss[3];
		ShaderType type = ShaderType::NONE;

		while (getline(stream, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
				else if (line.find("geometry") != std::string::npos)
					type = ShaderType::GEOMETRY;
			}
			else {
				ss[(int)type] << line << "\n";
			}
		}

		return { ss[0].str(), ss[1].str(), ss[2].str() };
	}
	static unsigned int compileShader(unsigned int type, const std::string& source) {
		GLCall(unsigned int id = glCreateShader(type));
		const char* src = source.c_str();
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		int result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE) {
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(id, length, &length, message));
			std::cout << "FAILED TO COMPILE ";
			switch (type) {
			case GL_VERTEX_SHADER:
				std::cout << "VERTEX ";
			case GL_FRAGMENT_SHADER:
				std::cout << "FRAGMENT ";
			case GL_GEOMETRY_SHADER:
				std::cout << "GEOMETRY ";
			}
			std::cout << "SHADER" << std::endl;
			std::cout << message << std::endl;
			GLCall(glDeleteShader(id));
			return 0;
		}

		return id;

	}
	static unsigned int createShader(const std::string& vs, const std::string& fs, const std::string& gs) {

		GLCall(unsigned int program = glCreateProgram());
		unsigned int vertShader = compileShader(GL_VERTEX_SHADER, vs);
		unsigned int fragShader = compileShader(GL_FRAGMENT_SHADER, fs);
		unsigned int geomShader = 0;

		if (!gs.empty()) { //make sure the program has this
			geomShader = compileShader(GL_GEOMETRY_SHADER, gs);
			GLCall(glAttachShader(program, geomShader));
		}

		GLCall(glAttachShader(program, vertShader));
		GLCall(glAttachShader(program, fragShader));
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vertShader));
		GLCall(glDeleteShader(fragShader));
		GLCall(glDeleteShader(geomShader));

		return program;

	}
	
	Shader::Shader(const std::string& filepath) {
		ShaderProgramSource src = parseShader(filepath);
		shaderID = createShader(src.VertexSource, src.FragmentSource, src.GeometrySource);

	}

	Shader::~Shader() {

		/*
			BUG_REPORT: in main.cpp the shader is being deleted after the window closes.
			causing the subsequent calls to openGL functions to error and cause an infinite loop
		*/

		GLCall(glDeleteProgram(shaderID));

	}

	void Shader::bind() {
		GLCall(glUseProgram(shaderID));
	}

	void Shader::unbind() {
		GLCall(glUseProgram(0));
	}

	int Shader::getUniformLocation(const char* identifier) {
		int out;
		GLCall(out = glGetUniformLocation(shaderID, identifier));
		return out;
	}

	void Shader::setUniformVec2(const int& location, const math::vec2& data) {
		GLCall(glUniform2fv(location, 1, &data.x));
	}

	void Shader::setUniformVec3(const int& location, const math::vec3& data) {
		GLCall(glUniform3fv(location, 1, &data.x));
	}

	void Shader::setUniformVec4(const int& location, const math::vec4& data) {
		GLCall(glUniform4fv(location, 1, &data.x));
	}

	void Shader::setUniformMat4(const int& location, const math::mat4& data) {
		GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, data.elements));
	}

} }
