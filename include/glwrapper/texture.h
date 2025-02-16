#pragma once
#include "glwrapper/common.h"
#include "util/macro.h"

#include "gl3.h"
#include <string>

class Texture : public GLObject {
public:
	NO_COPY(Texture);
	MOVE(Texture);
	Texture(const std::string& path, int wrap_s = GL_REPEAT,
	        int wrap_t = GL_REPEAT, int min_filter = GL_LINEAR,
	        int mag_filter = GL_LINEAR);
	virtual ~Texture();
};
