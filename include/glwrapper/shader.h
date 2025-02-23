#pragma once
#include "glwrapper/gl_object.h"
#include "util/macro.h"

class Shader : public GLObject {
public:
	NO_COPY(Shader);
	MOVE(Shader);
	static Shader create(const char* vertex_shader_path, const char* flagment_shader_path);
	virtual ~Shader();

	void use() const;

private:
	Shader(GLuint id);
};
