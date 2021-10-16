#pragma once
#include "glwrapper/common.h"
#include "util/macro.h"

class Shader : public GLObject {
public:
	NO_COPY(Shader);
	Shader(unsigned type, const char* path);
	Shader(Shader&& another);
	Shader& operator=(Shader&& another);
	virtual ~Shader();
};

class ShaderProgram : public GLObject {
public:
	NO_COPY(ShaderProgram);
	ShaderProgram(Shader&& vertex_shader, Shader&& fragment_shader);
	ShaderProgram(ShaderProgram&& another);
	ShaderProgram& operator=(ShaderProgram&& another);
	void use() const;
	virtual ~ShaderProgram();
};
