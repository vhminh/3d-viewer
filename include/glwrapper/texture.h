#pragma once
#include "glwrapper/gl_object.h"
#include "util/macro.h"

class Texture : public GLObject {
public:
	NO_COPY(Texture);
	MOVE(Texture);
	static Texture create(const char* path, int wrap_s = GL_REPEAT, int wrap_t = GL_REPEAT, int min_filter = GL_LINEAR,
	                      int mag_filter = GL_LINEAR);
	virtual ~Texture();

private:
	Texture(GLuint id);
};
