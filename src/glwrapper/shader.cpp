#include "glwrapper/shader.h"

#include "gl3.h"
#include "util/error.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

// reference to one vertext shader or fragment shader
class SingleShader : public GLObject {
public:
	SingleShader(GLint id) : GLObject(id) {}

	NO_COPY(SingleShader);

	SingleShader(SingleShader&& another) : GLObject(another.id) { another.id = GLObject::ID_NONE; }
	Shader& operator=(Shader&& another) = delete;

	static SingleShader create(GLenum type, const char* path) {
		// read shader from file
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
		GLuint id = glCreateShader(type);
		SingleShader shader = SingleShader(id); // RAII
		glShaderSource(id, 1, &src, nullptr);
		// compile
		glCompileShader(shader.id);
		// check compile status
		int success;
		glGetShaderiv(shader.id, GL_COMPILE_STATUS, &success);
		if (!success) {
			char err_msg[1024];
			glGetShaderInfoLog(shader.id, 1024, NULL, err_msg);
			throw ShaderCompilationException(std::string("Error compile shader from source file \"") + path +
			                                 "\": " + std::string(err_msg));
		}
		return shader;
	}
	~SingleShader() {
		if (id != GLObject::ID_NONE)
			glDeleteShader(id);
	}
};

Shader Shader::create(const char* vertex_shader_path, const char* fragment_shader_path) {
	GLuint id = glCreateProgram();

	// attach shaders
	SingleShader vertex = SingleShader::create(GL_VERTEX_SHADER, vertex_shader_path);
	SingleShader fragment = SingleShader::create(GL_FRAGMENT_SHADER, fragment_shader_path);
	glAttachShader(id, vertex.get_id());
	glAttachShader(id, fragment.get_id());

	// link
	glLinkProgram(id);
	Shader shader = Shader(id); // RAII

	int success;
	glGetProgramiv(shader.get_id(), GL_LINK_STATUS, &success);
	if (!success) {
		char err_msg[2048];
		glGetProgramInfoLog(shader.get_id(), 2048, NULL, err_msg);
		throw ProgramLinkageException(std::string("Error link program: ") + std::string(err_msg));
	}
	return shader;
}

Shader::Shader(GLuint id) : GLObject(id) {}

Shader::Shader(Shader&& another) : GLObject(another.id) { another.id = GLObject::ID_NONE; }

Shader& Shader::operator=(Shader&& another) {
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

void Shader::use() const { glUseProgram(id); }

void Shader::setUniformBool(const char* name, bool value) {
	GLint loc = glGetUniformLocation(id, name);
	glUniform1i(loc, value);
}

void Shader::setUniformInt(const char* name, int value) {
	GLint loc = glGetUniformLocation(id, name);
	glUniform1i(loc, value);
}

void Shader::setUniformFloat(const char* name, float value) {
	GLint loc = glGetUniformLocation(id, name);
	glUniform1f(loc, value);
}

void Shader::setUniformVec3(const char* name, const glm::vec3& value) {
	GLint loc = glGetUniformLocation(id, name);
	glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::setUniformVec3(const char* name, const float* values) {
	GLint loc = glGetUniformLocation(id, name);
	glUniform3fv(loc, 1, values);
}

void Shader::setUniformMat4(const char* name, const glm::mat4& value) {
	GLint loc = glGetUniformLocation(id, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniformTexture(const char* name, GLint texture_slot) {
	GLint loc = glGetUniformLocation(id, name);
	glUniform1i(loc, texture_slot);
}

Shader::~Shader() {
	if (this->id != GLObject::ID_NONE) {
		glDeleteProgram(this->id);
	}
}
