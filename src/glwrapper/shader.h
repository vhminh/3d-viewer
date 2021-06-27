#pragma once
#include "common.h"

class Shader : public GLObject {
public:
    Shader(unsigned type, const char* path);
    Shader(Shader const&) = delete;
    Shader& operator=(Shader const&) = delete;
    virtual ~Shader();
};

class ShaderProgram : public GLObject {
public:
    ShaderProgram() = default;
    ShaderProgram(const char* vertex_shader_path, const char* fragment_shader_path);
    ShaderProgram(const Shader &vertex_shader, const Shader &fragment_shader);
    ShaderProgram(ShaderProgram const&) = delete;
    ShaderProgram& operator=(ShaderProgram const&) = delete;
    void move_from(ShaderProgram &another);
    void use() const;
    virtual ~ShaderProgram();
};
