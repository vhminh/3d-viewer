#include "glwrapper/shader.h"

#include "util/error.h"

#include "gl3.h"
#include <fstream>

Shader::Shader(unsigned type, const char* path) {
	// read shader source code from file
	std::ifstream fin(path);
	if (!fin.is_open()) {
		throw FileNotFoundException(path);
	}
	std::string line;
	std::string file_content;
	while (getline(fin, line)) {
		file_content.append(line).append("\n");
	}
	const char* src = file_content.c_str();
	// create shader
	this->id = glCreateShader(type);
	glShaderSource(this->id, 1, &src, nullptr);
	// compile
	glCompileShader(this->id);
	// check compile status
	int success;
	glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
	if (!success) {
		char err_msg[1024];
		glGetShaderInfoLog(this->id, 1024, NULL, err_msg);
		throw ShaderCompilationException(
			std::string("Error compile shader from source file \"") + path +
			"\": " + std::string(err_msg));
	}
}

Shader::~Shader() {
	if (this->id != ID_NONE) {
		glDeleteShader(this->id);
	}
}

Shader::Shader(Shader&& another) {
	this->id = another.id;
	another.id = ID_NONE;
}

Shader& Shader::operator=(Shader&& another) {
	if (this == &another) {
		return *this;
	}
	if (this->id != ID_NONE) {
		glDeleteShader(this->id);
	}
	this->id = another.id;
	another.id = ID_NONE;
	return *this;
}

ShaderProgram::ShaderProgram(Shader&& vertex_shader, Shader&& fragment_shader) {
	this->id = glCreateProgram();
	// attach shaders
	glAttachShader(this->id, vertex_shader.get_id());
	glAttachShader(this->id, fragment_shader.get_id());
	// link
	glLinkProgram(this->id);
	int success;
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success) {
		char err_msg[2048];
		glGetProgramInfoLog(this->id, 2048, NULL, err_msg);
		throw ProgramLinkageException(std::string("Error link program: ") +
		                              std::string(err_msg));
	}
}

ShaderProgram::ShaderProgram(ShaderProgram&& another) {
	this->id = another.id;
	another.id = ID_NONE;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& another) {
	if (this == &another) {
		return *this;
	}
	if (this->id != GLObject::ID_NONE) {
		glDeleteProgram(this->id);
	}
	this->id = another.id;
	another.id = GLObject::ID_NONE;
	return *this;
}

void ShaderProgram::use() const { glUseProgram(id); }

ShaderProgram::~ShaderProgram() {
	if (this->id != GLObject::ID_NONE) {
		glDeleteProgram(this->id);
	}
}
