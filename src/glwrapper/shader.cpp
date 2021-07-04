#include "shader.h"
#include <fstream>
#include <GLES3/gl3.h>
#include "../util/error.h"

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
        throw ShaderCompilationException(std::string("Error compile shader from source file \"") + path + "\": " + std::string(err_msg));
    }
}

Shader::~Shader() {
    glDeleteShader(this->id);
}

ShaderProgram::ShaderProgram(const char *vertex_shader_path, const char *fragment_shader_path)
    : ShaderProgram(Shader(GL_VERTEX_SHADER, vertex_shader_path), Shader(GL_FRAGMENT_SHADER, fragment_shader_path)) {}

ShaderProgram::ShaderProgram(const Shader &vertex_shader, const Shader &fragment_shader) {
    this->id = glCreateProgram();
    // attach shaders
    glAttachShader(this->id, vertex_shader.get_id());
    glAttachShader(this->id, fragment_shader.get_id());
    // link
    glLinkProgram(this->id);
    int success;
    glGetProgramiv(this->id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char err_msg[2048];
        glGetProgramInfoLog(this->id, 2048, NULL, err_msg);
        throw ProgramLinkageException(std::string("Error link program: ") + std::string(err_msg));
    }
}

void ShaderProgram::move_from(ShaderProgram &another) {
    if (this->id != GLObject::ID_NONE) {
        glDeleteProgram(this->id);
    }
    this->id = another.id;
    another.id = GLObject::ID_NONE;
}

void ShaderProgram::use() const {
    glUseProgram(id);
}

ShaderProgram::~ShaderProgram() {
    if (this->id != GLObject::ID_NONE) {
        glDeleteProgram(this->id);
    }
}

