#pragma once
#include "glwrapper/gl_object.h"
#include "util/macro.h"

#include <glm/glm.hpp>

class Shader : public GLObject {
public:
	NO_COPY(Shader);
	MOVE(Shader);
	static Shader create(const char* vertex_shader_path, const char* flagment_shader_path);
	virtual ~Shader();

	void use() const;
	void setUniformVec3(const char* name, const glm::vec3& value);
	void setUniformMat4(const char* name, const glm::mat4& value);
	void setUniformTexture(const char* name, GLint texture_id);

private:
	Shader(GLuint id);
};
